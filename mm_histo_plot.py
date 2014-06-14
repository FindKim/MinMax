#!/usr/bin/python -tt

import re
import sys
import os

class mm_histo_plot():
	def __init__(self, fileName):
		self.fileName = fileName
		self.plotFileName = fileName.strip('.csv') + '.gp'
		self.psFileName = fileName.strip('.csv') + '.ps'
		self.pdfFileName = fileName.strip('.csv') + '.pdf'
		self.sequences = [] # Null list

	def get_fileName(self):
		return self.fileName
	
	def get_plotFileName(self):
		return self.plotFileName
	
	def get_psFileName(self):
		return self.psFileName
	
	def get_pdfFileName(self):
		return self.pdfFileName
	
	# Calculate number of sequences to plot
	def get_numOfSeqs(self):
		file_in = open(self.fileName)
		firstLine = file_in.readline()
		file_in.close()
		numOfSeqs = len(re.findall(',', firstLine))
		return numOfSeqs
	
	# Parse string into list of sequence names; delimiter:','
	def get_seqsNameList(self):
		file_in = open(self.fileName)
		firstLine = file_in.readline()
		file_in.close()
		return firstLine.split(',')
	
	# Linux touch files
	def touch(self, fname):
		try:
			os.utime(fname, None)
		except:
			open(fname, 'a').close()

	# Dumps gnuplot formatting for stacked histograms
	def format_histo(self):
		fout = open(self.plotFileName, 'w')
		fout.write('set terminal postscript portrait\nset output \"%s\"\n' %(self.psFileName))
		fout.write('\n# Top & bottom margin\nset tmargin 0\nset bmargin 1\n')
		fout.write('\n# Left & right margin to have same size plots\nset lmargin 5\nset rmargin 5\n')	
		fout.write('\n# Data columns are separated with ,\nset datafile separator ","\n')
		fout.write('\n# Turn off xtics for all plots but the bottom one\nunset xtics\nunset ytics\n')
		fout.write('\n# Displays origin of x-axis\nset xzeroaxis lt 1 linecolor rgb "#000000"\n')
		fout.write('\nset yrange[-100:100]\n#set format y "rare	common"\n#set ytics (" Common" 50,  "Rare" -50)\n#set ytics axis in scale 0,0 nomirror rotate by 90 font "Helvetica,10"\n#set ytics font "Helvetica,5"\n')
		fout.write('set key autotitle columnhead\nset key inside left bottom vertical nobox\nset key samplen -1\n')
		fout.write('set multiplot layout 9, 1 title "MinMax plots for %s"\nset style histogram rowstacked\nset style data histogram\n' %(self.fileName))
		fout.write('\n# Histogram bars filled with solid color and black border\nset style fill solid noborder\n')
		fout.close()

	# Dynamically dumps columns to be plotted
	def input_histo(self):
		fout = open(self.plotFileName, 'a')
		for i in range(1, 10): # gnuplot begins counting rows at 1 (9 plots)
			fout.write('plot \'' + self.fileName + '\' using ($' + str(i) + ' > 0 ? $' + str(i) + ' : 0) linecolor rgb \"#0000FF\", \'\' using ($' + str(i) + ' < 0 ? $' + str(i) + ' : -0.05) title \'\' linecolor rgb \"#FF00FF\"\n')
#		seqList = get_seqsNameList()
#		for seq in seqList:
#		seqList = self.get_seqsNameList()

		fout.close()
		fin = open(self.fileName, 'r')
		fin.close()

	# "Compiles" gnuplot file and formats output to pdf
	def plot_histo(self):
		command1 = 'gnuplot %s' %(self.plotFileName)
		command2 = 'ps2pdf %s' %(self.psFileName)
		os.system(command1)
		os.system(command2)
	
	# Deletes .gp and .ps files user will not need
	def del_temp_files(self):
		command = 'rm %s %s' %(self.psFileName, self.plotFileName)
		os.system(command)

def main():
#	print str(sys.argv[1])
	fileName = str(sys.argv[1])
	histo = mm_histo_plot(fileName)
	print 'The number of sequences: %d' %(histo.get_numOfSeqs())
	histo.touch(histo.get_plotFileName())
	histo.format_histo()
	histo.input_histo()
	histo.plot_histo()
	histo.del_temp_files()
	print '%s has been created.' %(histo.get_pdfFileName())

if __name__ == '__main__':
	main()
