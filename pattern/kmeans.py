# —*- coding: utf-8 -*-
#! /usr/bin/env python

import numpy as np
import random as rd
import matplotlib.pyplot as plt

# @param x, c: np.array
def D(x, c):
	tmp = x-c
	return np.sum(tmp*tmp, axis=1)	

# @param class : np.array (matrix)
def update(cl):
	cnt = np.shape(cl)[0]
	sum = np.sum(cl, axis=0)
	represent = sum / cnt * 1.0
	return represent

def judge(current, prev, threshold):
	if False in (D(current, prev) < threshold):
		return False
	else:
		return True

# @param c: np.array
# @param k: int
def init(c,k):
	represent = []
	random = rd.randint(0,c.shape[0]-1)
	represent.append(c[random])
	c = np.delete(c,random,axis=0)
	for i in range(k)[1:]:
		dist = []
		for x in c:
			dist.append(np.min(D(x, represent))**2)
		dist = np.array(dist)
		dist = dist / np.sum(dist)*1.0
		y = 1
		u = 0
		while y > dist[u]:
			y = rd.uniform(np.min(dist), np.max(dist))
			u = rd.randint(0,c.shape[0]-1)
		represent.append(c[u])
		c = np.delete(c,u,axis=0)
	return represent
		
	

# main
c = []
for line in open("data.txt", "r"):
	data = line.split(",")
	for i in range(len(data)-1):
		data[i] = float(data[i])
	c.append(data[:4])
c = np.array(c)

colors = ['b','g','r','k','w']

for k in [2,3,4,5]:
	while True:
		represent = init(c,k)
		classified = [[] for i in range(k)]
		for x in c:
			j = np.argmin(D(x,represent))
			classified[j].append(x)
		current = []
		for i in range(k):
			current.append(update(classified[i]))
		if judge(np.array(current),np.array(represent),0.1):
			break
		else:
			represent = current
	for i in range(k):
		plt.scatter(np.array(classified[i]).T[0],np.array(classified[i]).T[1], c=colors[i])
	plt.show()
	
# test
A = np.array([[1,2,3],[2,3,1]])
B = np.array([[2,1,3],[2,2,3]])


