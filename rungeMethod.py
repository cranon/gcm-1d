#!/usr/bin/python

import sys
import os
import math
import shutil
import math
import time
import numpy as np
import matplotlib.pyplot as pl
from scipy import interpolate

def norm(f,x):
	N = x.size
	h = np.empty(N)
	h[0] = (x[1] - x[0])/2
	h[N-1] = (x[N-1] - x[N-2])/2
	for i in range(1,N-1,1):
		h[i] = (x[i+1] - x[i-1])/2
	tmp = f*f*h
	a = np.sum(tmp)
	return np.sqrt(a)	

def foo1(NumT, maxTau, m):
	os.system('./run -n ' + str(NumT) + ' -t ' + str(maxTau) + ' -m ' + str(m))
	
def getNumT(TauIsFixed, i):
	if (TauIsFixed):
		return 1
	else:
		return pow(2, i)

def getTau(TauIsFixed, Num, TauMin, i):
	if(TauIsFixed): 
		return 0.9*TauMin
	else:
		return 0.9*TauMin*pow(2, Num - 1 - i)

def main():	
	if (sys.argv[1] == '-n'):
		Num = int(sys.argv[2])
		if (sys.argv[3] == '-m'):
			methodType = float(sys.argv[4])
	elif (sys.argv[1] == '-m'):
		methodType = float(sys.argv[2])
		if (sys.argv[3] == '-n'):
			Num = int(sys.argv[4])
	else:
		print 'Invalid Parameters!\n'
		sys.exit(-1)
	
	TauIsFixed = True
	
	if (sys.argv[5] == '-c'):
		TauIsFixed = False
	
	if (os.path.exists("data")): 
		shutil.rmtree("data")
		os.mkdir("data")
	else:
		os.mkdir("data")
	
	if (os.path.exists("ApprOrder")):
	        shutil.rmtree("ApprOrder")
	        os.mkdir("ApprOrder")
	else:
	        os.mkdir("ApprOrder")
	
	cfgFile = open('body1.txt','r')
	cfg = cfgFile.readlines()
	cfgFile.close()
	
	a = math.sqrt(float(cfg[17].split()[1])/float(cfg[16].split()[1]))
	LeftCnr = float(cfg[14].split()[1])
	RightCnr = float(cfg[15].split()[1])
	NumofNodes = []
	NormDiff = []
	
	InitNumOfNodes = 50
	N = InitNumOfNodes
	h = (RightCnr - LeftCnr)/N
	hmin = h/pow(2, Num-1)
	NumofNodes += [N]
	cfg[13] = cfg[13].split()[0] + '\t' + str(N+1) + '\n'
	out = open('body1.txt','w')
	out.writelines(cfg)
	out.close()
	
	foo1(getNumT(TauIsFixed, 0), getTau(TauIsFixed, Num, hmin/a, 0), methodType)
	
	fileName = 'data/Body_1_Step_' + str(getNumT(TauIsFixed, 0)) + '.txt'
	while True:
		if os.path.exists(fileName):
			dataFile = open(fileName, 'r')
			data = dataFile.readlines()
			dataFile.close()
			break
		time.sleep(0.1)
	j = 0
	X = np.empty(N+1)
	W = np.empty(N+1)
	for tmp in data:
		X[j] = float(tmp.split()[1])
		W[j] = float(tmp.split()[7])	
		j = j + 1
	
	
	
	for i in range(1, Num):
		N = InitNumOfNodes*pow(2,i)
		h = (RightCnr - LeftCnr)/N
		NumofNodes += [N]
		cfg[13] = cfg[13].split()[0] + '\t' + str(N+1) + '\n'
		out = open('body1.txt','w')
		out.writelines(cfg)
		out.close()
		
		if (os.path.exists("data")): 
			shutil.rmtree("data")
			os.mkdir("data")
		else:
			os.mkdir("data")
		
		foo1(getNumT(TauIsFixed, i), getTau(TauIsFixed, Num, hmin/a, i), methodType)
		
		fileName = 'data/Body_1_Step_' + str(getNumT(TauIsFixed, i)) + '.txt'
		while True:
			if os.path.exists(fileName):
				dataFile = open(fileName, 'r')
				data = dataFile.readlines()
				dataFile.close()
				break
			time.sleep(0.1)
			
		j = 0
		x = np.empty(N+1)
		w = np.empty(N+1)
		for tmp in data:
			x[j] = float(tmp.split()[1])
			w[j] = float(tmp.split()[7])	
			j = j + 1
		
		f = interpolate.interp1d(x, w, kind='cubic')
		w1 = f(X) 
		NormDiff += [norm(w1 - W, X)]
		
		#pl.figure(i)
		#pl.title('N = ' + str(N+1))
		#pl.grid(True)
		#pl.plot(X, w1, 'bo', x, w, '-', X, W, 'ro')
		#pl.savefig('ApprOrder/interp1d_' + str(i) + '.png')
		#pl.close("all")
		
		pl.figure(i)
		pl.grid(True)
		pl.plot(X, w1 - W)
		pl.savefig('ApprOrder/comparison_' + str(i) + '.png')
		pl.close("all")
		W = np.empty(np.size(w))
		W = w
		X = np.empty(np.size(x))
		X = x
	logDiff = []
	for i in range(1, Num-1):
		logDiff += [math.log(NormDiff[i-1]/NormDiff[i])/math.log(2)]
	
	pl.figure()
	pl.grid(True)
	pl.plot(range(0, Num-1), NormDiff, '*-')
	pl.savefig('ApprOrder/NormGraph.png')
	print NormDiff
	print logDiff
	pl.figure()
	pl.grid(True)
	pl.plot(range(0, Num-2), logDiff, '*-')
	pl.savefig('ApprOrder/logGraph.png')
main()
