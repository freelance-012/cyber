#include "modules/common_msgs/sensor_msgs/sensor_image.pb.h"

#include "cyber/cyber.h"
#include <opencv2/opencv.hpp>

void MessageCallback(
    const std::shared_ptr<apollo::drivers::Image>& msg) {
  // AINFO << "Received message: \n";
  // AINFO << "frame_id: " << msg->frame_id();
  // AINFO << "measurement_time: " << std::to_string(msg->measurement_time());
  // AINFO << "width: " << msg->width();
  // AINFO << "height: " << msg->height();
  // AINFO << "step: " << msg->step();
  // AINFO << "encoding: " << msg->encoding();
  // AINFO << "data.size: " << msg->data().size();
  // AINFO << "-----------------------------";

  cv::Mat encoded(msg->data().size(), 1, CV_8UC1, (uint8_t*)(msg->data().data()));
  cv::Mat image = cv::imdecode(encoded, cv::IMREAD_COLOR);

  cv::imshow("image", image);
  cv::waitKey(80);

}

int main(int argc, char* argv[]) {
  // init cyber framework
  apollo::cyber::Init(argv[0]);
  // create listener node
  auto listener_node = apollo::cyber::CreateNode("listener");

  // create listener
  auto listener =
      listener_node->CreateReader<apollo::drivers::Image>(
          "channel/image", MessageCallback);
  apollo::cyber::WaitForShutdown();
  return 0;
}
