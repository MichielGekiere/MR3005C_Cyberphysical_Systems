#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <cmath>

int main(int argc, char** argv)
{
  ros::init(argc, argv, "signal_generator");

  ros::NodeHandle nodehandle;

  ros::Publisher signal_pub = nodehandle.advertise<std_msgs::Float32>("/signal", 10);
  ros::Publisher time_pub = nodehandle.advertise<std_msgs::Float32>("/time", 10);

  ros::Rate rate(10);

  float t = 0.0;

  while (ros::ok())
  {
    float y = sin(t);

    std_msgs::Float32 signal_msg;
    signal_msg.data = y;
    signal_pub.publish(signal_msg);

    std_msgs::Float32 time_msg;
    time_msg.data = t;
    time_pub.publish(time_msg);

    ROS_INFO("t => %.3f, signal => %.3f", t, y);

    t = t + 0.1;
    rate.sleep();
  }

  return 0;
}