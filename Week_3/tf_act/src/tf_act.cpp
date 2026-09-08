#include <cmath>

#include <geometry_msgs/TransformStamped.h>
#include <ros/ros.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/static_transform_broadcaster.h>
#include <tf2_ros/transform_broadcaster.h>

geometry_msgs::TransformStamped sun_tf;
geometry_msgs::TransformStamped planet_tf;

void initSunTransform()
{
  sun_tf.header.frame_id = "inertial_frame";
  sun_tf.child_frame_id = "sun";
  sun_tf.header.stamp = ros::Time::now();
  sun_tf.transform.translation.x = 1.0;
  sun_tf.transform.translation.y = 1.0;
  sun_tf.transform.translation.z = 1.0;
  sun_tf.transform.rotation.x = 0.0;
  sun_tf.transform.rotation.y = 0.0;
  sun_tf.transform.rotation.z = 0.0;
  sun_tf.transform.rotation.w = 1.0;
}

void initPlanetTransform()
{
  planet_tf.header.frame_id = "sun";
  planet_tf.child_frame_id = "planet";
  planet_tf.header.stamp = ros::Time::now();
  planet_tf.transform.translation.x = 0.0;
  planet_tf.transform.translation.y = 0.0;
  planet_tf.transform.translation.z = 0.0;
  planet_tf.transform.rotation.x = 0.0;
  planet_tf.transform.rotation.y = 0.0;
  planet_tf.transform.rotation.z = 0.0;
  planet_tf.transform.rotation.w = 1.0;
}

void stop()
{
  ROS_INFO("Stopping");
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "RVIZ_marker");
  ros::NodeHandle nh;

  ros::Rate loop_rate(10);

  initSunTransform();
  initPlanetTransform();

  tf2_ros::StaticTransformBroadcaster bc_sun;
  tf2_ros::TransformBroadcaster bc_planet;

  ROS_INFO("The tf's are ready");

  while (ros::ok())
  {
    const double t = ros::Time::now().toSec();

    sun_tf.header.stamp = ros::Time::now();

    tf2::Quaternion q;
    q.setRPY(0.0, M_PI * t, 0.0);

    planet_tf.header.stamp = ros::Time::now();
    planet_tf.transform.translation.x = 1.2 * std::sin(t);
    planet_tf.transform.translation.y = 1.2 * std::cos(t);
    planet_tf.transform.translation.z = 0.0;
    planet_tf.transform.rotation.x = q.x();
    planet_tf.transform.rotation.y = q.y();
    planet_tf.transform.rotation.z = q.z();
    planet_tf.transform.rotation.w = q.w();

    bc_sun.sendTransform(sun_tf);
    bc_planet.sendTransform(planet_tf);

    loop_rate.sleep();
  }

  stop();
  return 0;
}
