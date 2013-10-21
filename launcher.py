#!/usr/bin/python
import commands
import os
import shutil
import sys
import math

# Script for running cpp programm and making videos and plots

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

if (os.path.exists("data")): 
	shutil.rmtree("data")
	os.mkdir("data")
	os.mkdir("data/plots")
else:
	os.mkdir("data")
	os.mkdir("data/plots")

print commands.getoutput('./run -n ' + str(NumT) + ' -t ' + str(maxTau))	

cfgFile = open('body1.txt','r')
cfg = cfgFile.readlines()
cfgFile.close()
MaxV = math.sqrt((float(cfg[18].split()[1]))**2 + (float(cfg[17].split()[1]))/(float(cfg[16].split()[1]))*(float(cfg[19].split()[1]))**2);
MaxEps = math.sqrt((float(cfg[19].split()[1]))**2 + (float(cfg[16].split()[1]))/(float(cfg[17].split()[1]))*(float(cfg[18].split()[1]))**2);
rheology = cfg[1].split()[0]
if (rheology == 'plastic'):
	MaxV = MaxV*3
LeftCnr = float(cfg[5].split()[1]);
RightCnr = float(cfg[6].split()[1]);

if MaxV != 0:
	plt = open('plotter','r')
	plt_lines = plt.readlines()
	plt.close()
	for i in range(len(plt_lines)):
		line_arr = plt_lines[i].split()
		if line_arr.count("yrange") == 1:
			line_arr[2] = str( '[-' + str(2*MaxV) + ':' + str(2*MaxV) + ']' )
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
			line_arr[2] = str( '"data/plots/Plot_' + str(i/step) + '.jpeg"')
			plt_lines[j] = ''
			for word in line_arr:
				plt_lines[j] = plt_lines[j] + word + ' '
			plt_lines[j] = plt_lines[j] + '\n'
		if line_arr.count("plot") == 1:
			line_arr[1] = str( '"data/Body_1_Step_' + str(i/step) + '.txt"')
			plt_lines[j] = ''
			for word in line_arr:
				plt_lines[j] = plt_lines[j] + word + ' '
			plt_lines[j] = plt_lines[j] + '\n'
			
	plt = open('plotter','w')
	plt.writelines(plt_lines)
	plt.close()
	text = commands.getoutput('gnuplot plotter')

text = commands.getoutput('avconv -y -f image2 -i data/plots/Plot_%d.jpeg -r 12 video.avi')
text = commands.getoutput('totem video.avi')

