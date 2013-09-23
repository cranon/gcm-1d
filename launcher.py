#!/usr/bin/python
import commands
import os
import shutil
import sys
import math
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("")
shutil.rmtree("data/plots")
os.mkdir("data/plots")

cfgFile = open('body1.txt','r')
cfg = cfgFile.readlines()
cfgFile.close()
MaxV = math.sqrt(float(cfg[16].split()[1])*float(cfg[16].split()[1]) + float(cfg[23].split()[1])*float(cfg[23].split()[1]) + float(cfg[17].split()[1])*float(cfg[17].split()[1]) + float(cfg[24].split()[1])*float(cfg[24].split()[1]))/4 

if MaxV != 0:
	plt = open('plotter','r')
	plt_lines = plt.readlines()
	plt.close()
	for i in range(len(plt_lines)):
		line_arr = plt_lines[i].split()
		if line_arr.count("yrange") == 1:
			line_arr[2] = str( '[-' + str(MaxV) + ':' + str(MaxV) + ']' )
			plt_lines[i] = ''
			for word in line_arr:
				plt_lines[i] = plt_lines[i] + word + ' '
			plt_lines[i] = plt_lines[i] + '\n'
			
	plt = open('plotter','w')
	plt.writelines(plt_lines)
	plt.close()


step = 1
NumTimeSteps = 400
for i in range(0,(NumTimeSteps + 1),step):
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

