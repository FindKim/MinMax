#!/usr/bin/python -tt

# mmplot created by Kim Ngo
# June 6, 2014
#
# mmplot reads in .mm files created by main program and offers the option to plot min max values for specified genes as heat maps or stacked histograms
#
#

import mm_histo_plot

import string
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
	print('\nPlotting Menu:')
	print('	1. Stacked histograms')
	print('	2. Heatmaps')
	print('	3. Input another .mm file')
	print('	4. List all .mm files in dir')
	print('	5. Exit')

	# Check if valid selection
	selection = raw_input('Make a selection from the list above:	')
	try:
		selection = int(selection)
		return selection
	except ValueError or NameError:
			print 'Invalid selection'
			return 0
	except KeyboardInterrupt:
		sys.exit()

def gnuplot(fname):
#	print 'attempting to link gnuplot'
	os.system('clear')
	os.system('mm_histo_plot.py %s' %(fname))

def r(fname):
	os.system('clear')
	print 'attempting to link R'



def main():
	while(1):
		try:
			filePath = raw_input('\nEnter a .mm file to plot if the file is in this working directory.\nOtherwise, enter the absolute path.\n--\'m\' for menu.\n--\'q\' to exit.\n>')

		# Check if file exists and has valid file extension
			validFileExt = re.compile('.+\.mm')
			if validFileExt.match(filePath) and os.path.isfile(filePath):

				selection = menu()
				while(1):
		
					# Stacked histograms
					if selection == 1:
						gnuplot(filePath)
						selection = menu()
		
					# Heatmaps
					elif selection == 2:
						r(filePath)
						selection = menu()
				
					# Another file to plot
					elif selection == 3:
						break
					
					# List all .mm files in directory
					elif selection == 4:
						lsDir = raw_input('Enter the absolute path to directory of interest or \'.\' for this current working directory.\n>')
						os.system('ls %s | awk \\/.mm/' %(lsDir))
						selection = menu()
					
					# Exit
					elif selection == 5:
						sys.exit()
	
					# Invalid selection
					else:
						os.system('clear')
						selection = menu()

			elif filePath == 'm':
				selection = menu()
			elif filePath == 'q':
				sys.exit()
			else:
				print '**Invalid file**'
				continue
				
		except KeyboardInterrupt:
			sys.exit()

if __name__ == '__main__':
	main()

