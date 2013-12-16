#!/usr/bin/python
import commands
import os
import shutil
import threading
import sys
import math
import time

import matplotlib.pyplot as pl
#import numpy as np
#from scipy import integrate

#	Script for checking amplitudes in layer structure
#	
#	! ! ! ! ! W A R N I N G ! ! ! ! !
#
#	In Parser.cpp Layer structure is on!	
#	body1.txt is like LS_Body1.txt
#	run like launcher.py, but tau must be normal
#

def writeDefault():
	cfgFile = open('body1.txt','r')
	cfg = cfgFile.readlines()
	cfgFile.close()
	
	cfg[1] = 'elastic' + '\n'
	cfg[4] = cfg[4].split()[0] + '\t' + str(1e+6) + '\n'
	cfg[5] = cfg[5].split()[0] + '\t' + 'V' + '\t' + str(0) + '\n'
	cfg[9] = cfg[9].split()[0] + '\t' + str(1e+6) + '\n'
	cfg[10] = cfg[10].split()[0] + '\t' + 'V' + '\t' + str(0) + '\n'
	cfg[13] = cfg[13].split()[0] + '\t' + str(501) + '\n'
	cfg[14] = cfg[14].split()[0] + '\t' + str(0) + '\n'
	cfg[15] = cfg[15].split()[0] + '\t' + str(0.1) + '\n'
	cfg[16] = cfg[16].split()[0] + '\t' + str(7.5e+3) + '\n'
	cfg[17] = cfg[17].split()[0] + '\t' + str(2e+11) + '\n'
	if (cfg[22].split()[0] == "gauss"):
		cfg[22] = '#' + cfg[22]
	cfg[23] = cfg[23].split()[0] + '\t' + str(0.005) + '\n'
	cfg[24] = cfg[24].split()[0] + '\t' + str(0.015) + '\n'
	cfg[25] = cfg[25].split()[0] + '\t' + str(7.5e+3) + '\n'
	cfg[26] = cfg[26].split()[0] + '\t' + str(2e+11) + '\n'
	cfg[27] = cfg[27].split()[0] + '\t' + str(10) + '\n'
	cfg[28] = cfg[28].split()[0] + '\t' + str(0) + '\n'
		
	out = open('body1.txt','w')
	out.writelines(cfg)
	out.close()
	
	
def foo1(NumT, maxTau, m):
	os.system('./run -n ' + str(NumT) + ' -t ' + str(maxTau) + ' -m ' + str(m))

if (sys.argv[1] == '-n'):
	NumT = int(sys.argv[2])
	if (sys.argv[3] == '-t'):
		maxTau = float(sys.argv[4])
elif (sys.argv[1] == '-t'):
	maxTau = float(sys.argv[2])
	if (sys.argv[3] == '-n'):
		NumT = int(sys.argv[4])
else:
	print 'Invalid Parameters!\n'
	sys.exit(-1)

if (sys.argv[5] == '-m'):
	methodType = int(sys.argv[6])

if (sys.argv[7] == '-default'):
	writeDefault()	

if (os.path.exists("data")): 
	shutil.rmtree("data")
	os.mkdir("data")
	os.mkdir("data/plots")
else:
	os.mkdir("data")
	os.mkdir("data/plots")
	
thrd1 = threading.Thread(target = foo1, args = (NumT, maxTau, methodType))
thrd1.start()

cfgFile = open('body1.txt','r')
cfg = cfgFile.readlines()
cfgFile.close()

Number_of_nodes = int(cfg[13].split()[1])
LeftCnr = float(cfg[14].split()[1])
RightCnr = float(cfg[15].split()[1])
rho0 = float(cfg[16].split()[1])
E0 = float(cfg[17].split()[1])
x1 = float(cfg[23].split()[1])
x2 = float(cfg[24].split()[1])
V0 = float(cfg[27].split()[1])/2
Eps0 = float(cfg[28].split()[1])

def getRho(i):
	return i*rho0
	
NumOfLayers = 5
x_side = []
for i in range(1, NumOfLayers):
	x_side += [i*(RightCnr - LeftCnr)/NumOfLayers]

a = [V0]
fileNumber = [0]
x = x1
for i in range(1, NumOfLayers):
	a += [2*a[i-1]/(1 + math.sqrt(getRho(i+1)/getRho(i)))]
	fileNumber += [fileNumber[i-1] + int((x_side[i-1] - x)/maxTau/math.sqrt(E0/getRho(i)))]
	x = x_side[i-1]

if fileNumber[NumOfLayers-1] > NumT:
	print "note: need more steps for fully checking amplitudes"
	quit

u = [V0]
D = [0]
H = int((x2 - x1)*(Number_of_nodes-1)/(RightCnr - LeftCnr))
for i in range(1, NumOfLayers):
	fileName = 'data/Body_1_Step_' + str(fileNumber[i]) + '.txt'
	while True:
		if os.path.exists(fileName):
			cfgFile = open(fileName,'r')
			cfg = cfgFile.readlines()
			cfgFile.close()
			break
		time.sleep(0.1)
	H = int(H*math.sqrt(getRho(i)/getRho(i+1)))
	u += [float(cfg[int(x_side[i-1]*(Number_of_nodes-1)/(RightCnr - LeftCnr) + H/2)].split()[2])]
	D += [math.log10(abs(a[i] - u[i])/a[i])]
	print fileNumber[i], int(x_side[i-1]*(Number_of_nodes-1)/(RightCnr - LeftCnr) + H/2)

D.pop(0)
print a, u
pl.figure(1)
pl.xlabel('Layer_num')
pl.xlabel('log10(abs(an - num)/an)')
pl.title('Difference between analytic and simulated amplitudes')
pl.grid(True)
pl.plot(range(1, 5), D, 'o-')
pl.savefig('comparasion.png')
pl.show()

		#pl.figure(1)
		#pl.title('Method ' + str(m) + ' d = ' + str(d) + ' (Gauss)')
		#pl.grid(True)
		#pl.plot(NofN, normDif, '*-')
		#pl.savefig('ApprOrder/' + str(d) + '/' + str(m) + '/err.png')
		#pl.close("all")
		#pl.figure(2)
		#pl.title('Method ' + str(m) + ' d = ' + str(d) + ' (Gauss)')
		#pl.grid(True)
		#pl.plot(np.log2(normDif), '*-')
		#pl.savefig('ApprOrder/' + str(d) + '/' + str(m) + '/log2.png')
		#pl.show()

		#cfg[13] = cfg[13].split()[0] + '\t' + str(InitNumOfNodes) + '\n'
		#plt = open('body1.txt','w')
		#plt.writelines(cfg)
		#plt.close()

