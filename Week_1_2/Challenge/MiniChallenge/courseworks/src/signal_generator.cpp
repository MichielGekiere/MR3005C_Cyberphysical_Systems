#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <cmath>

// argc = argument count, argv = argument vector/values
int main(int argc, char** argv)
{
  // starts node named "signal_generator"
  ros::init(argc, argv, "signal_generator");

  // create node handle (for communicating with ROS)
  ros::NodeHandle nh;

  // node-private param: time step between samples (default 0.1 s -> 10 Hz)
  ros::NodeHandle private_nh("~");
  float time_step = 0.1;
  private_nh.param("time_step", time_step, 0.1f);

  // create a var publisher named x
  // this var will publish/send messages to topic y, with queue size of 10
  ros::Publisher signal_pub = nh.advertise<std_msgs::Float32>("/signal", 10);
  ros::Publisher time_pub = nh.advertise<std_msgs::Float32>("/time", 10);
  
  // set speed from time_step (e.g. 0.1 s -> 10 Hz)
  ros::Rate rate(1.0 / time_step);

  float t = 0.0;

  while (ros::ok())
  {
    float y = sin(t);

    // std_msg => standard message, Float32 (the message) => data type float32
    std_msgs::Float32 signal_msg;
    signal_msg.data = y;
    signal_pub.publish(signal_msg);

    std_msgs::Float32 time_msg;
    time_msg.data = t;
    time_pub.publish(time_msg);

    // round to 3 decimal places
    ROS_INFO("t => %.3f, signal => %.3f", t, y);

    t = t + time_step;  // advance time by one step each loop
    rate.sleep();
  }

  return 0;
}
