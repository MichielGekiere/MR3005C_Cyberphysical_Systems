#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <cmath>

// globals for callbacks and then using it in the main function
float signal_data = 0.0;
float time_data = 0.0;
bool got_signal = false;
bool got_time = false;

// best practice: callback functions to get data from signal and time topics
void signalCallback(const std_msgs::Float32::ConstPtr& msg)
{
  signal_data = msg->data;
  got_signal = true;
}

// we receive std_msgs::Float32 message
// ConstPtr is a pointer to a constant message (we only read, best practice)
// use pointers instead of copying the message data (performance)
void timeCallback(const std_msgs::Float32::ConstPtr& msg)
{
  time_data = msg->data;
  got_time = true;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "process");

  // default values if params are not set in the launch file
  float global_offset = 1.0;
  float group_offset = 1.0;
  float node_offset = 1.0;
  float phase = M_PI / 2.0;
  float amplitude_reduction = 0.5;

  // 1. global param: full path on the param server (/global_offset)
  ros::NodeHandle nh;
  nh.param("/global_offset", global_offset, 1.0f);

  // 2. group param: one level up from this node (/Group1/group_offset)
  ros::NodeHandle group_nh(nh, "..");
  group_nh.param("group_offset", group_offset, 1.0f);

  // 3. node-private params: only for this node (/Group1/process/...)
  ros::NodeHandle private_nh("~");
  private_nh.param("node_offset", node_offset, 1.0f);
  private_nh.param("phase", phase, static_cast<float>(M_PI / 2.0));
  private_nh.param("amplitude_reduction", amplitude_reduction, 0.5f);

  // use the private offset for processing (Group1 -> 1.0, Group2 -> 2.0)
  float offset = node_offset;

  ROS_INFO("params => global: %.1f, group: %.1f, node: %.1f, phase: %.3f, amp: %.1f",
           global_offset, group_offset, node_offset, phase, amplitude_reduction);

  ros::Subscriber signal_sub = nh.subscribe("/signal", 10, signalCallback);
  ros::Subscriber time_sub = nh.subscribe("/time", 10, timeCallback);

  ros::Publisher proc_pub = nh.advertise<std_msgs::Float32>("/proc_signal", 10);

  ros::Rate rate(10);

  while (ros::ok())
  {
    // check for new messages
    ros::spinOnce();

    // if we have both signal and time data
    if (got_signal && got_time)
    {
      // phase shift: sin(t + phase) = sin(t)*cos(phase) + cos(t)*sin(phase)
      float shifted = signal_data * cos(phase) + cos(time_data) * sin(phase);

      // offset
      float offset_wave = shifted + offset;

      // half the amplitude
      float processed = amplitude_reduction * offset_wave;

      // publish the processed signal
      std_msgs::Float32 proc_msg;
      proc_msg.data = processed;
      proc_pub.publish(proc_msg);

      ROS_INFO("proc_signal => %.3f", processed);
    }

    rate.sleep();
  }

  return 0;
}
