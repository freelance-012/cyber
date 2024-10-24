
#include "modules/common_msgs/sensor_msgs/sensor_image.pb.h"

#include "cyber/cyber.h"
#include "cyber/time/rate.h"
#include "cyber/time/time.h"

#include <opencv4/opencv2/opencv.hpp>

using apollo::cyber::Rate;
using apollo::cyber::Time;
using apollo::drivers::Image;

int main(int argc, char *argv[]) {
  // init cyber framework
  apollo::cyber::Init(argv[0]);
  // create talker node
  auto talker_node = apollo::cyber::CreateNode("talker");
  // create talker
  auto talker = talker_node->CreateWriter<Image>("channel/image");
  Rate rate(20.0);
  uint64_t seq = 0;
  AINFO << "begin while";
  while (apollo::cyber::OK()) {
    cv::Mat image = cv::imread("/tmp/lena.jpg", cv::IMREAD_UNCHANGED);

    cv::putText(image, std::to_string(seq), cv::Point(30, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 255), 3);

    // AINFO << "image.size: " << image.size() << std::endl;
    std::vector<uint8_t> encoded_image;
    cv::imencode(".jpg", image, encoded_image);

    // 获取当前时间
    apollo::cyber::Time now = apollo::cyber::Time::Now();
    double ts = now.ToSecond();

    auto msg = std::make_shared<Image>();
    msg->set_frame_id(std::to_string(seq));
    msg->set_measurement_time(ts);
    msg->set_width(image.cols);
    msg->set_height(image.rows);
    msg->set_encoding("jpeg");
    msg->set_step(image.cols * image.channels());
    msg->set_data(std::string(encoded_image.begin(), encoded_image.end()));
    talker->Write(msg);
    // AINFO << "talker sent a message! No. " << seq;
    // AINFO << "-----------------------------";
    seq++;
    rate.Sleep();
  }
  return 0;
}
