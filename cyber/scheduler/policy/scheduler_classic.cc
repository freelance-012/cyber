/******************************************************************************
 * Copyright 2018 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#include "cyber/scheduler/policy/scheduler_classic.h"

#include <algorithm>
#include <memory>
#include <utility>

#include "cyber/common/environment.h"
#include "cyber/common/file.h"
#include "cyber/scheduler/policy/classic_context.h"
#include "cyber/scheduler/processor.h"

namespace apollo {
namespace cyber {
namespace scheduler {

using apollo::cyber::base::ReadLockGuard;
using apollo::cyber::base::WriteLockGuard;
using apollo::cyber::common::GetAbsolutePath;
using apollo::cyber::common::GetProtoFromFile;
using apollo::cyber::common::GlobalData;
using apollo::cyber::common::PathExists;
using apollo::cyber::common::WorkRoot;
using apollo::cyber::croutine::RoutineState;

/**
 * Step1. 解析调度配置文件
 * Step2. 创建Processor
 */
SchedulerClassic::SchedulerClassic() {
  std::string conf("conf/");
  conf.append(GlobalData::Instance()->ProcessGroup()).append(".conf");
  auto cfg_file = GetAbsolutePath(WorkRoot(), conf);

  apollo::cyber::proto::CyberConfig cfg;
  if (PathExists(cfg_file) && GetProtoFromFile(cfg_file, &cfg)) {
    for (auto& thr : cfg.scheduler_conf().threads()) {
      inner_thr_confs_[thr.name()] = thr;
    }

    if (cfg.scheduler_conf().has_process_level_cpuset()) {
      process_level_cpuset_ = cfg.scheduler_conf().process_level_cpuset();
      ProcessLevelResourceControl();
    }

    classic_conf_ = cfg.scheduler_conf().classic_conf();
    for (auto& group : classic_conf_.groups()) {
      auto& group_name = group.name();
      for (auto task : group.tasks()) {
        task.set_group_name(group_name);
        cr_confs_[task.name()] = task;
      }
    }
  } else {
    // if do not set default_proc_num in scheduler conf
    // give a default value
    uint32_t proc_num = 2;
    auto& global_conf = GlobalData::Instance()->Config();
    if (global_conf.has_scheduler_conf() &&
        global_conf.scheduler_conf().has_default_proc_num()) {
      proc_num = global_conf.scheduler_conf().default_proc_num();
    }
    task_pool_size_ = proc_num;

    auto sched_group = classic_conf_.add_groups();
    sched_group->set_name(DEFAULT_GROUP_NAME);
    sched_group->set_processor_num(proc_num);
  }

  CreateProcessor();
}

/**
 * / 每创建一个Processor都会创建一个ClassicContext实例与之绑定,ClassicContext功能主要用于管理Processor内的协程任务。
 * / Processor相当于一个线程执行体，用于执行分配给它的协程任务
 * / SetSchedAffinity函数内其实就是调用pthread_setaffinity_np将线程绑定到指定的cpu上
 * / processor_policy参数可以配置线程不同的调度策略：
 * /    SCHED_OTHER: 分时调度策略，所有线程优先级为0，线程调度通过分时来完成
 * /    SCHED_FIFO: 实时的先进先出调度策略，表示可运行线程将一直抢占
 * /    SCHED_RR: 是SCHED_FIFO的一种改进，可以使用最大运行时间来限制当前线程的云心g
 * / processor_prio参数主要用于配置线程优先级
 * 
 */
void SchedulerClassic::CreateProcessor() {
  for (auto& group : classic_conf_.groups()) {
    /// group_name
    auto& group_name = group.name();
    /// proc_num
    auto proc_num = group.processor_num();
    if (task_pool_size_ == 0) {
      task_pool_size_ = proc_num;
    }

    /// affinity
    auto& affinity = group.affinity();
    /// processor_policy
    auto& processor_policy = group.processor_policy();
    /// processor_prio
    auto processor_prio = group.processor_prio();
    std::vector<int> cpuset;
    ParseCpuset(group.cpuset(), &cpuset);

    /// 根据proc_num数量创建processors_
    for (uint32_t i = 0; i < proc_num; i++) {
      /// ClassicContext创建
      auto ctx = std::make_shared<ClassicContext>(group_name);
      pctxs_.emplace_back(ctx);

      /// Processor创建
      auto proc = std::make_shared<Processor>();
      /// ctx绑定到proc
      proc->BindContext(ctx);
      /// SetSchedAffinity函数内其实就是调用pthread_setaffinity_np将线程绑定到指定cpu
      SetSchedAffinity(proc->Thread(), cpuset, affinity, i);
      /// processor_policy可以配置线程的调度策略，processor_prio用于配置线程优先级
      SetSchedPolicy(proc->Thread(), processor_policy, processor_prio,
                     proc->Tid());
      processors_.emplace_back(proc);
    }
  }
}

/**
 * / 主要工作是把一个新创建的协程任务加入协程池中
 */
bool SchedulerClassic::DispatchTask(const std::shared_ptr<CRoutine>& cr) {
  // we use multi-key mutex to prevent race condition
  // when del && add cr with same crid
  /// 依据协程id向id_map_mutex_内获取协程锁，
  MutexWrapper* wrapper = nullptr;
  if (!id_map_mutex_.Get(cr->id(), &wrapper)) { /// 没有就创建一个放入id_map_mutex_
    {
      std::lock_guard<std::mutex> wl_lg(cr_wl_mtx_);
      if (!id_map_mutex_.Get(cr->id(), &wrapper)) {
        wrapper = new MutexWrapper();
        id_map_mutex_.Set(cr->id(), wrapper);
      }
    }
  }

  /// 将协程放入协程map
  std::lock_guard<std::mutex> lg(wrapper->Mutex());
  {
    WriteLockGuard<AtomicRWLock> lk(id_cr_lock_);
    if (id_cr_.find(cr->id()) != id_cr_.end()) {
      return false;
    }
    id_cr_[cr->id()] = cr;
  }

  /// 依据调度配置参数设置协程的任务优先级，group_name等等
  if (cr_confs_.find(cr->name()) != cr_confs_.end()) {
    ClassicTask task = cr_confs_[cr->name()];
    cr->set_priority(task.prio());
    cr->set_group_name(task.group_name());
  } else {
    // croutine that not exist in conf
    cr->set_group_name(classic_conf_.groups(0).name());
  }

  if (cr->priority() >= MAX_PRIO) {
    AWARN << cr->name() << " prio is greater than MAX_PRIO[ << " << MAX_PRIO
          << "].";
    cr->set_priority(MAX_PRIO - 1);
  }

  /// 将协程放入对应的优先级队列
  // Enqueue task.
  {
    WriteLockGuard<AtomicRWLock> lk(
        ClassicContext::rq_locks_[cr->group_name()].at(cr->priority()));
    ClassicContext::cr_group_[cr->group_name()]
        .at(cr->priority())
        .emplace_back(cr);
  }

  /// 唤醒协程所属的group内的processor执行任务
  ClassicContext::Notify(cr->group_name());
  return true;
}

bool SchedulerClassic::NotifyProcessor(uint64_t crid) {
  if (cyber_unlikely(stop_)) {
    return true;
  }

  {
    ReadLockGuard<AtomicRWLock> lk(id_cr_lock_);
    if (id_cr_.find(crid) != id_cr_.end()) {
      auto cr = id_cr_[crid];
      if (cr->state() == RoutineState::DATA_WAIT ||
          cr->state() == RoutineState::IO_WAIT) {
        cr->SetUpdateFlag();
      }

      ClassicContext::Notify(cr->group_name());
      return true;
    }
  }
  return false;
}

bool SchedulerClassic::RemoveTask(const std::string& name) {
  if (cyber_unlikely(stop_)) {
    return true;
  }

  auto crid = GlobalData::GenerateHashId(name);
  return RemoveCRoutine(crid);
}

bool SchedulerClassic::RemoveCRoutine(uint64_t crid) {
  // we use multi-key mutex to prevent race condition
  // when del && add cr with same crid
  MutexWrapper* wrapper = nullptr;
  if (!id_map_mutex_.Get(crid, &wrapper)) {
    {
      std::lock_guard<std::mutex> wl_lg(cr_wl_mtx_);
      if (!id_map_mutex_.Get(crid, &wrapper)) {
        wrapper = new MutexWrapper();
        id_map_mutex_.Set(crid, wrapper);
      }
    }
  }
  std::lock_guard<std::mutex> lg(wrapper->Mutex());

  std::shared_ptr<CRoutine> cr = nullptr;
  {
    WriteLockGuard<AtomicRWLock> lk(id_cr_lock_);
    if (id_cr_.find(crid) != id_cr_.end()) {
      cr = id_cr_[crid];
      id_cr_[crid]->Stop();
      id_cr_.erase(crid);
    } else {
      return false;
    }
  }
  return ClassicContext::RemoveCRoutine(cr);
}

}  // namespace scheduler
}  // namespace cyber
}  // namespace apollo
