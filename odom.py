#!/usr/bin/env python
import rospy
from std_msgs.msg import Float32
from geometry_msgs.msg import Twist

wl = 0.0
wr = 0.0

def wl_cb(msg):
    global wl
    wl = msg.data


def wr_cb(msg):
    global wr
    wr = msg.data


def stop():
    print("Stopping odom node")


if __name__ == "__main__":
    rospy.init_node("odom")

    r = rospy.get_param("r", 0.05)  # [m] wheel radius
    d_wheels = rospy.get_param("d_wheels", 0.19) # [m]  distance between wheels [m] 

    rospy.Subscriber("/VelocityEncL", Float32, wl_cb)
    rospy.Subscriber("/VelocityEncR", Float32, wr_cb)
    pub = rospy.Publisher("/odom", Twist, queue_size=1)

    rate = rospy.Rate(10)
    rospy.on_shutdown(stop)

    try:
        while not rospy.is_shutdown():
            # Differential drive: encoders -> robot twist
            v = r * (wr + wl) / 2.0   # linear.x  [m/s]
            w = r * (wr - wl) / d_wheels  # angular.z [rad/s]

            msg = Twist()
            msg.linear.x = v
            msg.angular.z = w
            pub.publish(msg)

            rate.sleep()
    except rospy.ROSInterruptException:
        pass
