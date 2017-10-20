# -*- coding: utf-8 -*-
#! /usr/bin/env python
import matplotlib.pyplot as plt

# 距離関数
# @param x, xi: array (vector)
def D(x, xi):
	ret = 0
	for i in range(len(x)):
		ret += (x[i]-xi[i])**2
	return ret

# @param D: function
# @param k: int
# @param x: array (vector)
# @param c: array (matrix)
def min_k(k, x, c):
	min = []
	for i in range(k):
		min.append([D(x[:4], c[i][:4]), c[i][4]])
	for i in range(len(c)):
		min.append([D(x[:4], c[i][:4]), c[i][4]])
		min.sort(key=lambda x:x[0])
		min = min[:k]
	count = [0,0,0]
	tot = [0,0,0]
	for array in min:
		tot[array[1]] = tot[array[1]]+array[0]
		count[array[1]] = count[array[1]]+1
	max = count[0]
	max_index = 0
	for i in range(len(count)):
		if max < count[i]:
			max = count[i]
			max_index = i
		elif max == count[i]:
			if tot[max_index] > tot[i]:
				max = count[i]
				max_index = i
	return max_index

def accuracy(k, c):
	correct = 0
	for i in range(len(c)):
		if min_k(k, c[i], c[:i]+c[i+1:]) == c[i][4]:
			correct = correct+1
	return 1.0*correct/len(c)

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
