
find_package(OpenCV REQUIRED)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
)

add_executable(cyber_example_image_msg_talker image_msg_talker.cc ../../../modules/common_msgs/sensor_msgs/sensor_image.pb.cc)
target_link_libraries(cyber_example_image_msg_talker
    ${TARGET_NAME}
    pthread
    ${OpenCV_LIBS}
)

add_executable(cyber_example_image_msg_listener image_msg_listener.cc ../../../modules/common_msgs/sensor_msgs/sensor_image.pb.cc)
target_link_libraries(cyber_example_image_msg_listener
    ${TARGET_NAME}
    pthread
    ${OpenCV_LIBS}
)
