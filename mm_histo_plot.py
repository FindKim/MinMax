#!/usr/bin/python -tt

import string # Replace extensions
import re	# Regex
import sys
import os
import math # Ceiling function
from collections import deque # Deque of pdf to merge
	
def get_plotFileName(fileName, num):
	return string.replace(fileName, '.mm', str(num) + '_histo.gp')
	
def get_psFileName(fileName, num):
	return string.replace(fileName, '.mm', str(num) + '_histo.ps')
	
def get_pdfFileName(fileName, num):
	return string.replace(fileName, '.mm', str(num) + '_histo.pdf')
	
# Calculate number of sequences to plot
def get_numOfSeqs(fileName):
	file_in = open(fileName)
	firstLine = file_in.readline()
	file_in.close()
	numOfSeqs = len(re.findall(',', firstLine))
	return numOfSeqs

# Parse string into list of sequence names; delimiter:','
def get_seqsNameList():
	file_in = open(fileName)
	firstLine = file_in.readline()
	file_in.close()
	return firstLine.split(',')

# Linux touch files
def touch(fname):
	try:
		os.utime(fname, None)
	except:
		open(fname, 'a').close()

# Dumps gnuplot formatting for stacked histograms
def format_histo(fileName, plotFileName, psFileName, fileNum, totNumFile):
	fout = open(plotFileName, 'w')
	fout.write('set terminal postscript portrait\nset output \"%s\"\n' %(psFileName))
	fout.write('\n# Top & bottom margin\nset tmargin 0\nset bmargin 1\n')
	fout.write('\n# Left & right margin to have same size plots\nset lmargin 5\nset rmargin 5\n')	
	fout.write('\n# Data columns are separated with ,\nset datafile separator ","\n')
	fout.write('\n# Turn off xtics for all plots but the bottom one\nunset xtics\nunset ytics\n')
	fout.write('\n# Displays origin of x-axis\nset xzeroaxis lt 1 linecolor rgb "#000000"\n')
	fout.write('\nset yrange[-100:100]\n#set format y "rare	common"\n#set ytics (" Common" 50,  "Rare" -50)\n#set ytics axis in scale 0,0 nomirror rotate by 90 font "Helvetica,10"\n#set ytics font "Helvetica,5"\n')
	fout.write('set key autotitle columnhead\nset key inside left bottom vertical nobox\nset key samplen -1\n')
	fout.write('set multiplot layout 10, 1 title "%%MinMax plots for %s"\nset style histogram rowstacked\nset style data histogram\n' %(string.replace(fileName,'.mm', ' (%d/%d)' %(fileNum, totNumFile))))
	fout.write('\n# Histogram bars filled with solid color and black border\nset style fill solid noborder\n')
	fout.close()

# Dynamically dumps columns to be plotted
def input_histo(fileName, plotFileName, mult):
	fout = open(plotFileName, 'a')
	for i in range(1+10*mult, 11+10*mult): # gnuplot begins counting rows at 1 (9 plots)
		fout.write('plot \'' + fileName + '\' using ($' + str(i) + ' > 0 ? $' + str(i) + ' : 0) linecolor rgb \"#0000FF\", \'\' using ($' + str(i) + ' < 0 ? $' + str(i) + ' : -0.05) title \'\' linecolor rgb \"#FF00FF\"\n')
#		seqList = get_seqsNameList()
#		for seq in seqList:
#		seqList = self.get_seqsNameList()

	fout.close()


# "Compiles" gnuplot file and formats output to pdf
def plot_histo(plotFileName, psFileName):
	command1 = 'gnuplot %s' %(plotFileName)
	command2 = 'ps2pdf %s' %(psFileName)
	os.system(command1)
	os.system(command2)
	
# Deletes .gp and .ps files user will not need
def del_temp_files(fileName):
	fileroot = string.replace(fileName, '.mm', '')
	command = 'rm `ls | egrep \'%s[0-9]+_histo\'`' %(fileroot)
#	print command
	os.system(command)

def merge_pdf_files(files, mergedFile):
	command = 'convert '
	for pdfFile in files:
		command += (pdfFile + ' ')
	command += mergedFile
#	print command
	os.system(command)
	print '%s has been created.' %mergedFile

def main():
#	print str(sys.argv[1])
	fileName = str(sys.argv[1])
	files = deque()
	totNumFile = int(math.ceil(get_numOfSeqs(fileName)/10))+1
	mergedFile = string.replace(fileName, '.mm', '_histo.pdf')
	print 'Creating %s file...' %mergedFile
#	print 'The number of sequences: %d' %(histo.get_numOfSeqs())

	for numFile in range(0, totNumFile):
		plotFileName = get_plotFileName(fileName, numFile)
		psFileName = get_psFileName(fileName, numFile)
		pdfFileName = get_pdfFileName(fileName, numFile)
		touch(plotFileName)
		format_histo(fileName, plotFileName, psFileName, numFile+1, totNumFile)
		input_histo(fileName, plotFileName, numFile)
		plot_histo(plotFileName, psFileName)
		files.append(pdfFileName)
#		print '%s has been created.' %(pdfFileName)

	merge_pdf_files(files, mergedFile)
	del_temp_files(fileName)
	print '-------------All files have been created-------------'

if __name__ == '__main__':
	main()
