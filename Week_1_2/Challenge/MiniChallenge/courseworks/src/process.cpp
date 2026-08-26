#include <cmath>
#include <ros/ros.h>
#include <std_msgs/Float32.h>

namespace
{
const float kOffset = 1.0f;          // α so sin(t)+α stays >= 0
const float kPhase = 1.5707963f;     // φ = π/2, hardcoded as allowed
}

float g_signal = 0.0f;
float g_time = 0.0f;
bool g_got_signal = false;
bool g_got_time = false;

void signalCallback(const std_msgs::Float32::ConstPtr& msg)
{
  g_signal = msg->data;
  g_got_signal = true;
}

void timeCallback(const std_msgs::Float32::ConstPtr& msg)
{
  g_time = msg->data;
  g_got_time = true;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "process");
  ros::NodeHandle nh;

  ros::Subscriber signal_sub = nh.subscribe("/signal", 10, signalCallback);
  ros::Subscriber time_sub = nh.subscribe("/time", 10, timeCallback);
  ros::Publisher proc_pub = nh.advertise<std_msgs::Float32>("/proc_signal", 10);
  ros::Rate rate(10);

  while (ros::ok())
  {
    ros::spinOnce();

    if (g_got_signal && g_got_time)
    {
      // sin(t+φ) = sin(t)cos(φ) + cos(t)sin(φ)
      const float shifted = g_signal * std::cos(kPhase)
                          + std::cos(g_time) * std::sin(kPhase);
      const float processed = 0.5f * (shifted + kOffset);

      std_msgs::Float32 msg;
      msg.data = processed;
      proc_pub.publish(msg);

      ROS_INFO("proc_signal = %.3f", processed);
    }

    rate.sleep();
  }

  return 0;
}
