# —*- coding: utf-8 -*-
#! /usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt
import re
from mpl_toolkits.mplot3d import Axes3D



# データ読み込み→配列に格納
dataset = [] # [horsepower, weight]
label = [] # [mpg]
for line in open("auto-mpg.txt", "r"):
	error_flag = False
	data = re.split(" +", line)
	# data : [mpg, cylinders, displacement, horsepower, weight, acceleration, model year, origin, car name]
	data = data[:1]+data[3:5]
	for i in range(len(data)):
		try:
			data[i] = float(data[i])
		except ValueError:
			error_flag = True
	if error_flag:
		continue
	label.append(data[0])
	dataset.append(data[1:])
label = np.array(label)
dataset = np.array(dataset)

omega = np.linalg.inv(np.dot(dataset.T, dataset))
omega = np.dot(omega, dataset.T)
omega = np.dot(omega, label)

x = np.arange(np.min(dataset.T[0]), np.max(dataset.T[0]),(np.max(dataset.T[0])-np.min(dataset.T[0]))/15)
y = np.arange(np.min(dataset.T[1]), np.max(dataset.T[1]),(np.max(dataset.T[1])-np.min(dataset.T[1]))/15)
X,Y = np.meshgrid(x,y)

Z = omega[0]*X+omega[1]*Y

fig = plt.figure()
ax = Axes3D(fig)
ax.plot_wireframe(X,Y,Z)
ax.scatter3D(dataset.T[0],dataset.T[1], label)
plt.show()

from pca import *

# データ読み込み→配列に格納
dataset = [] # [horsepower, weight]
label = [] # [mpg]
for line in open("auto-mpg.txt", "r"):
	error_flag = False
	data = re.split(" +", line)
	# data : [mpg, cylinders, displacement, horsepower, weight, acceleration, model year, origin, car name]
	data = data[:1]+data[2:6]
	for i in range(len(data)):
		try:
			data[i] = float(data[i])
		except ValueError:
			error_flag = True
	if error_flag:
		continue
	label.append(data[0])
	dataset.append(data[1:])
label = np.array(label)
dataset = np.array(dataset)

dataset = PCA(dataset, 2)

omega = np.linalg.inv(np.dot(dataset.T, dataset))
omega = np.dot(omega, dataset.T)
omega = np.dot(omega, label)

x = np.arange(np.min(dataset.T[0]), np.max(dataset.T[0]),(np.max(dataset.T[0])-np.min(dataset.T[0]))/15)
y = np.arange(np.min(dataset.T[1]), np.max(dataset.T[1]),(np.max(dataset.T[1])-np.min(dataset.T[1]))/15)
X,Y = np.meshgrid(x,y)

Z = omega[0]*X+omega[1]*Y

fig = plt.figure()
ax = Axes3D(fig)
ax.plot_wireframe(X,Y,Z)
ax.scatter3D(dataset.T[0],dataset.T[1], label)
plt.show()
