#! /usr/bin/env python
# -*- coding: utf-8 -*-

import rospy, actionlib
from move_base_msgs.msg import *

if __name__ == '__main__':
	x = input("x: ")
	y = input("y: ")
	try:
		rospy.init_node('send_goal', anonymous=True)
		# client = actionlib.SimpleActionClient('move_base', MoveBaseAction)
		# client.wait_for_server()
		subclient = actionlib.SimpleActionClient('timing', MoveBaseAction)
		subclient.wait_for_server()

		goal = MoveBaseGoal()
		goal.target_pose.header.stamp = rospy.Time.now()
		goal.target_pose.header.frame_id = "/map"
		goal.target_pose.pose.position.x = x
		goal.target_pose.pose.position.y = y
		goal.target_pose.pose.orientation.w = 1
		print goal
		# client.send_goal(goal)
		subclient.send_goal(goal)
		# print client.wait_for_result()
		if subclient.wait_for_result()==False:
			print "time up"
			break
	except rospy.ROSInterruptException: pass
