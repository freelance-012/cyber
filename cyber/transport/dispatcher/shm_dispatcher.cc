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

#include "cyber/transport/dispatcher/shm_dispatcher.h"
#include "cyber/common/global_data.h"
#include "cyber/common/util.h"
#include "cyber/scheduler/scheduler_factory.h"
#include "cyber/transport/shm/readable_info.h"

namespace apollo {
namespace cyber {
namespace transport {

using common::GlobalData;

ShmDispatcher::ShmDispatcher() : host_id_(0) { Init(); }

ShmDispatcher::~ShmDispatcher() { Shutdown(); }

void ShmDispatcher::Shutdown() {
  if (is_shutdown_.exchange(true)) {
    return;
  }

  if (thread_.joinable()) {
    thread_.join();
  }

  {
    ReadLockGuard<AtomicRWLock> lock(segments_lock_);
    segments_.clear();
  }
}

void ShmDispatcher::AddSegment(const RoleAttributes& self_attr) {
  uint64_t channel_id = self_attr.channel_id();
  WriteLockGuard<AtomicRWLock> lock(segments_lock_);
  if (segments_.count(channel_id) > 0) {
    return;
  }
  auto segment = SegmentFactory::CreateSegment(channel_id);
  segments_[channel_id] = segment;
  previous_indexes_[channel_id] = UINT32_MAX;
}

void ShmDispatcher::ReadMessage(uint64_t channel_id, uint32_t block_index) {
  ADEBUG << "Reading sharedmem message: "
         << GlobalData::GetChannelById(channel_id)
         << " from block: " << block_index;
  /// 创建一个rb对象
  auto rb = std::make_shared<ReadableBlock>();
  /// 把block_index填入rb->index
  rb->index = block_index;
  /// 获取反序列化之后的msg数据
  if (!segments_[channel_id]->AcquireBlockToRead(rb.get())) {
    AWARN << "fail to acquire block, channel: "
          << GlobalData::GetChannelById(channel_id)
          << " index: " << block_index;
    return;
  }

  /// 获取反序列化之后的msg_info数据
  MessageInfo msg_info;
  const char* msg_info_addr =
      reinterpret_cast<char*>(rb->buf) + rb->block->msg_size();

  if (msg_info.DeserializeFrom(msg_info_addr, rb->block->msg_info_size())) {
    /// 调用OnMessage数据
    OnMessage(channel_id, rb, msg_info);
  } else {
    AERROR << "error msg info of channel:"
           << GlobalData::GetChannelById(channel_id);
  }
  segments_[channel_id]->ReleaseReadBlock(*rb);
}

void ShmDispatcher::OnMessage(uint64_t channel_id,
                              const std::shared_ptr<ReadableBlock>& rb,
                              const MessageInfo& msg_info) {
  if (is_shutdown_.load()) {
    return;
  }
  ListenerHandlerBasePtr* handler_base = nullptr;
  if (msg_listeners_.Get(channel_id, &handler_base)) {
    auto handler = std::dynamic_pointer_cast<ListenerHandler<ReadableBlock>>(
        *handler_base);
    handler->Run(rb, msg_info);
  } else {
    AERROR << "Cannot find " << GlobalData::GetChannelById(channel_id)
           << "'s handler.";
  }
}

void ShmDispatcher::ThreadFunc() {
  ReadableInfo readable_info;
  while (!is_shutdown_.load()) {
    /// 通过唯一的key获取共享内存中的readable_info数据
    if (!notifier_->Listen(100, &readable_info)) {
      ADEBUG << "listen failed.";
      continue;
    }

    if (readable_info.host_id() != host_id_) {
      ADEBUG << "shm readable info from other host.";
      continue;
    }

    uint64_t channel_id = readable_info.channel_id();
    uint32_t block_index = readable_info.block_index();

    {
      ReadLockGuard<AtomicRWLock> lock(segments_lock_);
      if (segments_.count(channel_id) == 0) {
        continue;
      }
      // check block index
      if (previous_indexes_.count(channel_id) == 0) {
        previous_indexes_[channel_id] = UINT32_MAX;
      }
      uint32_t& previous_index = previous_indexes_[channel_id];
      if (block_index != 0 && previous_index != UINT32_MAX) {
        if (block_index == previous_index) {
          ADEBUG << "Receive SAME index " << block_index << " of channel "
                 << channel_id;
        } else if (block_index < previous_index) {
          ADEBUG << "Receive PREVIOUS message. last: " << previous_index
                 << ", now: " << block_index;
        } else if (block_index - previous_index > 1) {
          ADEBUG << "Receive JUMP message. last: " << previous_index
                 << ", now: " << block_index;
        }
      }
      previous_index = block_index;

      ReadMessage(channel_id, block_index);
    }
  }
}

bool ShmDispatcher::Init() {
  host_id_ = common::Hash(GlobalData::Instance()->HostIp());
  /// 创建的notifier_ 使用唯一的key 映射到共享内存段
  notifier_ = NotifierFactory::CreateNotifier();
  /// reader端在初始化ShmDispatcher对象时会在这里启动一个线程
  thread_ = std::thread(&ShmDispatcher::ThreadFunc, this);
  scheduler::Instance()->SetInnerThreadAttr("shm_disp", &thread_);
  return true;
}

}  // namespace transport
}  // namespace cyber
}  // namespace apollo
