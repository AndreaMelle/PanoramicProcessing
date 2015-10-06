#!/usr/bin/python

import sys, getopt
import json
import shutil
import os
from os import listdir
from os.path import isfile, join
import subprocess

def main(argv):

	inDir = ''
	outDir = ''
	
	try:
		opts, args = getopt.getopt(argv, 'i:o:')
	except getopt.GetoptError:
		print 'invalid command line arguments'
		sys.exit(2)
		
	for opt, arg in opts:
		if opt == '-i':
			inDir = arg
		elif opt == '-o':
			outDir = arg

	if not inDir or not outDir:
		print 'invalid input or output directories'
		sys.exit(2)

	FNULL = open(os.devnull, 'w')    #use this if you want to suppress output to stdout from the subprocess
	filename = "my_file.dat"
	

	for f in listdir(inDir):
		inFilePath = join(inDir,f)
		if isfile(inFilePath):
			filename, file_extension = os.path.splitext(inFilePath)

			if not (file_extension == '.jpg' or file_extension == '.png'):
				continue

			outFilePath = join(outDir, f);

			process = "../builds/win/fisheye_to_equirectangular_batch/x64/Release/fisheye_to_equirectangular_batch.exe " + inFilePath + " -10 80 1250 220 " + outFilePath
			subprocess.call(process, stdout=FNULL, stderr=FNULL, shell=False)

			print inFilePath + ' >> ' + outFilePath


if __name__ == '__main__':
	main(sys.argv[1:])
