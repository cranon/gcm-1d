#!/usr/bin/python
import commands
import os
import shutil
import threading
import sys
import math
import time

# Script for running cpp programm and making videos and plots

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

isVelocity = False
if (sys.argv[7] == '-v'):
	isVelocity = True
	print 'Plots with velocity'

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
Vext = abs(float(cfg[5].split()[2]))
Epsext = abs(float(cfg[5].split()[2]))
V0 = math.sqrt((float(cfg[18].split()[1]))**2 + (float(cfg[17].split()[1]))/(float(cfg[16].split()[1]))*(float(cfg[19].split()[1]))**2);
Eps0 = math.sqrt((float(cfg[19].split()[1]))**2 + (float(cfg[16].split()[1]))/(float(cfg[17].split()[1]))*(float(cfg[18].split()[1]))**2);
MaxV = math.sqrt((float(cfg[27].split()[1]))**2 + (float(cfg[26].split()[1]))/(float(cfg[25].split()[1]))*(float(cfg[28].split()[1]))**2);
MaxEps = math.sqrt((float(cfg[28].split()[1]))**2 + (float(cfg[25].split()[1]))/(float(cfg[26].split()[1]))*(float(cfg[27].split()[1]))**2);
MaxV = math.sqrt(MaxV**2 + V0**2 + Vext**2)
MaxEps = math.sqrt(MaxEps**2 + Eps0**2 + Epsext**2)
if (not isVelocity):
	MaxV = MaxEps
plt = open('plotter','r')
plt_lines = plt.readlines()
plt.close()
for i in range(len(plt_lines)):
	line_arr = plt_lines[i].split()
	if line_arr.count("plot") == 1:
		if isVelocity:
			line_arr[3] = '2:3'
		else:
			line_arr[3] = '2:4'
		plt_lines[i] = ''
		for word in line_arr:
			plt_lines[i] = plt_lines[i] + word + ' '
		plt_lines[i] = plt_lines[i] + '\n'
plt = open('plotter','w')
plt.writelines(plt_lines)
plt.close()
rheology = cfg[1].split()[0]
LeftCnr = cfg[14].split()[1]
RightCnr = cfg[15].split()[1]

if MaxV != 0:
	plt = open('plotter','r')
	plt_lines = plt.readlines()
	plt.close()
	for i in range(len(plt_lines)):
		line_arr = plt_lines[i].split()
		if line_arr.count("yrange") == 1:
			line_arr[2] = str( '[-' + str(MaxV*0.2) + ':' + str(MaxV*0.8) + ']' )
			plt_lines[i] = ''
			for word in line_arr:
				plt_lines[i] = plt_lines[i] + word + ' '
			plt_lines[i] = plt_lines[i] + '\n'
		if line_arr.count("xrange") == 1:
			line_arr[2] = str( '[' + str(LeftCnr) + ':' + str(RightCnr) + ']' )
			plt_lines[i] = ''
			for word in line_arr:
				plt_lines[i] = plt_lines[i] + word + ' '
			plt_lines[i] = plt_lines[i] + '\n'

			
	plt = open('plotter','w')
	plt.writelines(plt_lines)
	plt.close()

step = 1
for i in range(0,(NumT + 1),step):
	plt = open('plotter','r')
	plt_lines = plt.readlines()
	plt.close()
	for j in range(len(plt_lines)):
		line_arr = plt_lines[j].split()
		if line_arr.count("output") == 1:
			line_arr[2] = str( '"data/plots/Plot_' + str(i) + '.jpeg"')
			plt_lines[j] = ''
			for word in line_arr:
				plt_lines[j] = plt_lines[j] + word + ' '
			plt_lines[j] = plt_lines[j] + '\n'
		if line_arr.count("plot") == 1:
			line_arr[1] = str( '"data/Body_1_Step_' + str(i) + '.txt"')
			plt_lines[j] = ''
			for word in line_arr:
				plt_lines[j] = plt_lines[j] + word + ' '
			plt_lines[j] = plt_lines[j] + '\n'
			
	plt = open('plotter','w')
	plt.writelines(plt_lines)
	plt.close()
	if (i == NumT):
		os.system('gnuplot plotter')
		os.system('avconv -y -f image2 -i data/plots/Plot_%d.jpeg -r 12 -threads auto video.avi')
		os.system('totem video.avi')
		exit(0)
	while True:
		if os.path.exists(str('data/Body_1_Step_' + str(i+1) + '.txt')):
			os.system('gnuplot plotter')
			break
		time.sleep(0.1)

