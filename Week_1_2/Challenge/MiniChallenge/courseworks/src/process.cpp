#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <cmath>

float signal_data = 0.0;
float time_data = 0.0;
bool got_signal = false;
bool got_time = false;

void signalCallback(const std_msgs::Float32::ConstPtr& msg)
{
  signal_data = msg->data;
  got_signal = true;
}

void timeCallback(const std_msgs::Float32::ConstPtr& msg)
{
  time_data = msg->data;
  got_time = true;
}

int main(int argc, char** argv)
{
  float offset = 1.0;
  float shift = M_PI / 2.0;
  float amplitude_reduction = 0.5;

  ros::init(argc, argv, "process");
  ros::NodeHandle nodehandle;

  ros::Subscriber signal_sub = nodehandle.subscribe("/signal", 10, signalCallback);
  ros::Subscriber time_sub = nodehandle.subscribe("/time", 10, timeCallback);

  ros::Publisher proc_pub = nodehandle.advertise<std_msgs::Float32>("/proc_signal", 10);

  ros::Rate rate(10);

  while (ros::ok())
  {
    ros::spinOnce();

    if (got_signal && got_time)
    {
      float shifted = signal_data * cos(shift) + cos(time_data) * sin(shift);

      float offset_wave = shifted + offset;

      float processed = amplitude_reduction * offset_wave;

      std_msgs::Float32 proc_msg;
      proc_msg.data = processed;
      proc_pub.publish(proc_msg);

      ROS_INFO("proc_signal => %.3f", processed);
    }

    rate.sleep();
  }

  return 0;
}
