#! /usr/bin/env python
# -*- coding: utf-8 -*-

import rospy, actionlib
from move_base_msgs.msg import *

if __name__ == '__main__':
	x = float(input("x: "))
	y = float(input("y: "))
	try:
		rospy.init_node('send_goal', anonymous=True)
		client = actionlib.SimpleActionClient('move_base', MoveBaseAction)
		client.wait_for_server()

		goal = MoveBaseGoal()
		goal.target_pose.header.stamp = rospy.Time.now()
		goal.target_pose.header.frame_id = "/map"
		goal.target_pose.pose.position.x = x
		goal.target_pose.pose.position.y = y
		goal.target_pose.pose.orientation.w = 1
		print goal
		client.send_goal(goal)

		finished_before_timeout = client.wait_for_result(rospy.Duration(5.0))
		if finished_before_timeout:
			print "finised before timeout"
		else:
			print "timeout"
			client.cancel_goal()
	except rospy.ROSInterruptException: pass