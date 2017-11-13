#! /usr/bin/env python
# -*- coding: utf-8 -*-

import rospy, actionlib
from move_base_msgs.msg import *
from opencv_apps.msg import RotatedRectStamped


def cb(msg):
	print msg.rect
	if msg.rect.center.x > 0 and msg.rect.center.y > 0:
		try:
			rospy.init_node('send_goal', anonymous=True)
			client = actionlib.SimpleActionClient('move_base', MoveBaseAction)
			client.wait_for_server()

			goal = MoveBaseGoal()
			goal.target_pose.header.stamp = rospy.Time.now()
			goal.target_pose.header.frame_id = "/map"
			goal.target_pose.pose.position.x = 3
			goal.target_pose.pose.position.y = 3
			goal.target_pose.pose.orientation.w = 1
			print goal
			client.send_goal(goal)
			print client.wait_for_result()
		except rospy.ROSInterruptException: pass


if __name__ == '__main__':
	rospy.init_node('send_goal', anonymous=True)
	rospy.Subscriber('/camshift/track_box', RotatedRectStamped, cb)

	rospy.spin()