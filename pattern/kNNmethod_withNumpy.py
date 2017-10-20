# -*- coding: utf-8 -*-
#! /usr/bin/env python
import matplotlib.pyplot as plt
import numpy as np

# 距離関数
# @param x: np.array (vector)
# @param c: np.array (matrix)
def D(x, c):
	tmp = x-c
	return np.sum(tmp*tmp, axis=1)

# @param D: function
# @param k: int
# @param x: array (vector)
# @param c: array (matrix)
def min_k(k, x, c):
	C = np.hsplit(c, [4])
	min = np.concatenate(([D(x,C[0]).T], [C[1].T[0]]), axis=0).T
	min = min[min[:,0].argsort(), :]
	count = [0,0,0]
	for array in min[:k]:
		count[int(array[1])] = count[int(array[1])]+1
	max = count[0]
	max_index = 0
	for i in range(len(count)):
		if max < count[i]:
			max = count[i]
			max_index = i
	return max_index

def accuracy(k, c):
	correct = 0
	for i in range(c.shape[0]):
		if min_k(k, np.delete(c[i], 4), np.delete(c, i, axis=0)) == c[i][4]:
			correct = correct+1
	return 1.0*correct/c.shape[0]

c = []
for line in open("data.txt", "r"):
	data = line.split(",")
	for i in range(len(data)-1):
		data[i] = float(data[i])
	if data[4] == 'Iris-setosa\n':
		data[4] = 0
	elif data[4] == 'Iris-versicolor\n':
		data[4] = 1
	else:
		data[4] = 2
	c.append(data)
c = np.array(c)

y = []
k = range(31)[1:]
for i in k:
	y.append(accuracy(i,c))

plt.plot(k, y, '-o')
plt.grid()
plt.xlabel("k")
plt.ylabel("accuracy")
plt.title("k-NearestNeighbor Method’s Accuracy")
plt.ylim([0.93,0.99])
plt.show()
