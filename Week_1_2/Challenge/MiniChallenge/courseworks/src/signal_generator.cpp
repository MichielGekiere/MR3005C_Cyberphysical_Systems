#include <cmath>
#include <ros/ros.h>
#include <std_msgs/Float32.h>

int main(int argc, char** argv)
{
  ros::init(argc, argv, "signal_generator");
  ros::NodeHandle nh;

  ros::Publisher signal_pub = nh.advertise<std_msgs::Float32>("/signal", 10);
  ros::Publisher time_pub = nh.advertise<std_msgs::Float32>("/time", 10);
  ros::Rate rate(10);

  const ros::Time t0 = ros::Time::now();

  while (ros::ok())
  {
    const float t = static_cast<float>((ros::Time::now() - t0).toSec());
    const float y = std::sin(t);

    std_msgs::Float32 signal_msg;
    signal_msg.data = y;
    signal_pub.publish(signal_msg);

    std_msgs::Float32 time_msg;
    time_msg.data = t;
    time_pub.publish(time_msg);

    ROS_INFO("t = %.3f, signal = %.3f", t, y);
    rate.sleep();
  }

  return 0;
}
