# -*- coding: utf-8 -*-
#! /usr/bin/env python

import numpy as np


# 共分散行列を求める
def Cov(X):
	ret = np.array([x - np.mean(x) for x in X.T]).T
	ret = np.dot(ret.T, ret) / ret.shape[0]
	return ret

# 固有値問題を解いて固有値の大きい順に固有値と固有ベクトルをソート
def sortedEigen(X):
	(val, vec) = np.linalg.eig(X)
	vec = vec.T
	ret_vec = []
	for (key, value) in enumerate(sorted(val, reverse=True)):
		ret_vec.append(vec[key])
	ret_vec = np.array(ret_vec)
	ret_val = np.array(sorted(val, reverse=True))
	return (ret_val, ret_vec)

# 線形写像
def linMap(X, eigvec, dim):
	components = eigvec[:dim]
	ret = np.dot(X, components.T)
	return ret

# 主成分分析
def PCA(X,dim):
	covmat = Cov(X)
	(eigval,eigvec) = sortedEigen(covmat)
	ret = linMap(X,eigvec,dim)
	return ret

