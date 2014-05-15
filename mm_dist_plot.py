#!/usr/bin/env python

#given an input file with mm values and their counts, creates a histogram of the
#mm values' counts. Saves to file and displays this image.

import matplotlib.pyplot as plt
import matplotlib as mpl
mpl.use('Agg')
import pylab
import numpy as np
import sys
import os

def main():
	start_dir = sys.argv[1]
	for filename in os.listdir(start_dir):	
		if filename.endswith(".dmm"):		
			print filename
			counts = getData(start_dir + filename)
			output = "mm_plots/" + filename.replace(".fasta.dmm","_dmm.png")
			bargraph(counts,output)

def bargraph(data,output):
    width=.8
    xaxis = np.arange(-100,100)
    
    plt.figure(1,(16,5))
    plt.subplot(111)

    plt.bar(xaxis,data,width,color='#ff9900',linewidth=0)

    plt.ylabel('Count')
    plt.xlabel('MinMax Value')
    plt.title('Distribution of MinMax Values')

    plt.savefig(output) #set a different filename if desired
    #plt.show()
    plt.close()

def getData(filename):
    f = open(filename,'r')
    lines = f.readlines()
    f.close()
    counts=[]

    for i in range(200):
        lines[i]=lines[i].split('\t')
        counts.append(int(lines[i][1]))
    return counts

if __name__ == "__main__":
	main()
