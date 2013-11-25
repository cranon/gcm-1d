#!/usr/bin/python

import sys
import os
import math
import shutil
import matplotlib.pyplot as pl
import numpy as np
from scipy import integrate

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

def initFunc(x):
	cfgFile = open('body1.txt','r')
	cfg = cfgFile.readlines()
	cfgFile.close()
	a = float(cfg[22].split()[2])
	sigma = float(cfg[22].split()[3])
	v0 = float(cfg[27].split()[1])
	A = math.sqrt(float(cfg[26].split()[1])/float(cfg[25].split()[1]))
	w = -2 * v0 * np.exp(-(x - a)*(x - a)/2/(sigma*sigma));
	return w

def initFunc2(x):
	cfgFile = open('body1.txt','r')
	cfg = cfgFile.readlines()
	cfgFile.close()
	v0 = float(cfg[27].split()[1])
	A = math.sqrt(float(cfg[26].split()[1])/float(cfg[25].split()[1]))
	w = np.zeros(x.size)
	l = float(cfg[23].split()[1])
	r = float(cfg[24].split()[1])
	z = (l <= x)*(x <= r)
	w = -2 * v0 * z
	return w

if (os.path.exists("ApprOrder")):
        shutil.rmtree("ApprOrder")
        os.mkdir("ApprOrder")
else:
        os.mkdir("ApprOrder")
NumOfRuns = int (sys.argv[1])

for m in [0,1,2]:
	for d in [0.989]: # [0.989, 0.75, 0.5, 0.11]:
		if (not os.path.exists("ApprOrder/" + str(d))):
			os.mkdir("ApprOrder/" + str(d))
		os.mkdir("ApprOrder/" + str(d) + "/" + str(m))
		cfgFile = open('body1.txt','r')
		cfg = cfgFile.readlines()
		cfgFile.close()
		InitNumOfNodes = int(cfg[13].split()[1])
		maxDif = np.empty(NumOfRuns)
		normDif = np.empty(NumOfRuns)
		NofN = np.empty(NumOfRuns)
		A = math.sqrt(float(cfg[26].split()[1])/float(cfg[25].split()[1]))
		for i in range(NumOfRuns):
			N = (InitNumOfNodes-1)*pow(2,i) + 1
			NofN[i] = N - 1
			cfg[13] = cfg[13].split()[0] + '\t' + str(N) + '\n'
			plt = open('body1.txt','w')
			plt.writelines(cfg)
			plt.close()
			h = (float(cfg[15].split()[1]) - float(cfg[14].split()[1]))/(N-1)
			t = d*h/A
			os.system('./run -n 1 -t ' + str(t) + ' -m ' + str(m) + ' > /dev/null')
			#if (i == NumOfRuns/2):
			#	os.system('./launcher.py -n 200 -t ' + str(t) + ' -m ' + str(m) + ' -v')
			x = np.empty(N)
			w = np.empty(N)
			bodyFile = open('data/Body_1_Step_1.txt','r')
			data = bodyFile.readlines()
			bodyFile.close()
			j = 0
			for tmp in data:
				x[j] = float(tmp.split()[1])
				w[j] = float(tmp.split()[7])	
				j = j + 1
			wExt = initFunc(x - A*t)
			wExt2 = initFunc2(x - A*t)
			dif = wExt2 - w
		#	'''
			pl.figure(i)
			pl.plot(x,dif)
			pl.savefig('ApprOrder/' + str(d) + '/' + str(m) + '/dif' + str(i) + '.png')
			pl.close("all")
		#	'''
			maxDif[i] = max(np.abs(dif))
			normDif[i] = norm(dif, x)

		pl.figure(1)
		pl.title('Method ' + str(m) + ' d = ' + str(d) + ' (Gauss)')
		pl.grid(True)
		pl.plot(NofN, normDif, '*-')
		pl.savefig('ApprOrder/' + str(d) + '/' + str(m) + '/err.png')
		pl.close("all")
		pl.figure(2)
		pl.title('Method ' + str(m) + ' d = ' + str(d) + ' (Gauss)')
		pl.grid(True)
		pl.plot(np.log2(normDif), '*-')
		pl.savefig('ApprOrder/' + str(d) + '/' + str(m) + '/log2.png')
		pl.show()

		cfg[13] = cfg[13].split()[0] + '\t' + str(InitNumOfNodes) + '\n'
		plt = open('body1.txt','w')
		plt.writelines(cfg)
		plt.close()

