#!/usr/bin/python

# mmplot created by Kim Ngo
# June 6, 2014
#
# mmplot reads in .mm files created by main program and offers the option to plot min max values for specified genes as heat maps or stacked histograms
#
#

import mm_histo_plot

import re
import os
import os.path
import sys

def touch(fname):
	try:
		os.utime(fname, None)
	except:
		open(fname, 'a').close()

def menu():
	try:
		print('\nPlotting Menu:')
		print('	1. Stacked histograms')
		print('	2. Heatmaps')
		print('	3. Exit')
		selection = int(input('Make a selection from the list above:	'))
		return selection
	
	except SyntaxError or NameError:
		print('Invalid selection.')
		return 0
	
	except KeyboardInterrupt:
		sys.exit()

def gnuplot(fname):
	print 'attempting to link gnuplot'
#	histo = histogram.histogram(fname)
		

def r(fname):
	print 'attempting to link R'


while(1):
	filePath = raw_input('Enter .mm file to plot if the file is in this working directory.\nOtherwise, enter the absolute path. \'q\' to exit.\n>')

	# Check if file exists and has valid file extension
	validFileExt = re.compile('.+\.mm')
	if validFileExt.match(filePath) and os.path.isfile(filePath):

		# Compiles transpose supporting c prog that transposes .mm file from rows to columns
		os.system('gcc transpose.c -o transpose')

		# Changes file extension from .mm to .csv
		csvFilePath = filePath.strip('.mm')
		csvFilePath = csvFilePath + '.csv'
		touch(csvFilePath)

		# Transposes rows of file to columns
		transposeFile = './transpose -t --fsep "," %s > %s' %(filePath, csvFilePath)
		os.system(transposeFile)

		selection = menu()
		while(1):
		
			# Stacked histograms
			if selection == 1:
				gnuplot(csvFilePath)
				selection = menu()
		
			# Heatmaps
			elif selection == 2:
				r(csvFilePath)
				selection = menu()
		
			# Exit
			elif selection == 3:
				sys.exit()
	
			# Invalid selection
			else:
				selection = menu()

	elif filePath == 'q':
		sys.exit()
	else:
		print '**Invalid file**\n'
		continue

