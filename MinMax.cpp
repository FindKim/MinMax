/*

	Created by Kim Ngo 14/4/7
	
	MinMax.h
	
	Uses CodonFrequency class for min, max, and actual freq to calculate minmax using Clark & Clarke method
	Sliding window of 17 codons within each orfeome.
	Using a 17 codon window to identify clusters of rare codons instead of individual rare codons

*/

#include "MinMax.h"
#include "Sequence.h"
#include "CodonFrequency.h"
#include <vector>
#include <fstream> // output file
#include <utility>
#include <string>
#include <stdlib.h> // system

const int WINDOWSIZE = 17;

using namespace std;

// Calculates MinMax values for each codon and calls outputFileMM
MinMax::MinMax(string filename, vector<Sequence> seq, CodonFrequency cf) {

	// Initializes maps with inherited maps from Codon Frequency
	vector<float> minMap = cf.getMinMap();
	vector<float> maxMap = cf.getMaxMap();
	vector<float> avgMap = cf.getAvgMap();
	float *codonFreq = cf.getCodonFreq();
	int *codonToAAMap = cf.getCodonToAAMap();
	
	
	// Calculates min max for each sequence in vector of seqs
	// Creates a vector of pairs for each sequence: name & vector of minmax values
	// Prints to output file
	for (int i = 0; i < seq.size(); i++) {

		vector<float> minMaxSeq = 
		calcMinMax(seq[i], minMap, maxMap, avgMap, codonFreq, codonToAAMap);

		string nameDescription = seq[i].getSeqName();
		nameDescription.append("|");
		nameDescription.append(seq[i].getSeqDescription());

		pair<string, vector<float> > temp(nameDescription, minMaxSeq);
		minMaxSequences.push_back(temp);
	}
	outputFileMM(filename, minMaxSequences);

	string mmfile = filename.erase(filename.size()-6, filename.size()); //".fasta"
	mmfile.append(".mm");	
	transposeOutput(mmfile);
}


// Calculates the max or min frequency for one sequence using a 17 codon window
vector<float> MinMax::calcMinMax(Sequence &seq, vector<float> &minMap, vector<float> &maxMap, vector<float> &avgMap, float *codonFreq, int *codonToAAMap) {

	int numCodonRep, AA;
	float minFreqWindowSum, maxFreqWindowSum, avgFreqWindowSum, actualFreqWindowSum;
	float minFreqWindowAvg, maxFreqWindowAvg, avgFreqWindowAvg, actualFreqWindowAvg;
	float percentMax, percentMin;
	vector<float> MinMaxValues;
	
	string seqStr = seq.getSeq();

	for (int i = 0; i < seq.getSeqLength()-WINDOWSIZE*3; i+=3) {
	
		minFreqWindowSum = 0;
		maxFreqWindowSum = 0;
		avgFreqWindowSum = 0;
		actualFreqWindowSum = 0;
	
		for (int j = 0; j < WINDOWSIZE*3; j+=3) {
		
			string triplet = seqStr.substr(i+j,3);
			numCodonRep = CodonFrequency::codonStrToBinaryRep(triplet);
			AA = codonToAAMap[numCodonRep];
//			cout << seqStr << endl;
//			cout << "numcodonRep = " << numCodonRep << endl;
//			cout << "AA " << AA << " " << codonToAAMap[numCodonRep] << endl;

			minFreqWindowSum += minMap[AA];
			maxFreqWindowSum += maxMap[AA];
			avgFreqWindowSum += avgMap[AA];
			actualFreqWindowSum += codonFreq[numCodonRep];
//			cout << "MIN " << triplet << " " << minMap[AA] << " " << minFreqWindowSum << endl;
//			cout << "MAX " << triplet << " " << maxMap[AA] << " " << maxFreqWindowSum << endl;
//			cout << "AVG " << triplet << " " << avgMap[AA] << " " << avgFreqWindowSum << endl;
//			cout << "ACT " << triplet << " " << codonFreq[numCodonRep] << " " << actualFreqWindowSum << endl;
		}
		
		minFreqWindowAvg = minFreqWindowSum / WINDOWSIZE;
		maxFreqWindowAvg = maxFreqWindowSum / WINDOWSIZE;
		avgFreqWindowAvg = avgFreqWindowSum / WINDOWSIZE;
		actualFreqWindowAvg = actualFreqWindowSum / WINDOWSIZE;
		
//		cout << "minFreqWindowAvg " << minFreqWindowAvg << endl;
//		cout << "maxFreqWindowAvg " << maxFreqWindowAvg << endl;
//		cout << "avgFreqWindowAvg " << avgFreqWindowAvg << endl;
//		cout << "actualFreqWindowAvg " << actualFreqWindowAvg << endl;
		
		// Calculates percentMax value
		percentMax = (actualFreqWindowAvg - avgFreqWindowAvg) / (maxFreqWindowAvg - avgFreqWindowAvg) * 100;
		
		// If percentMax value is negative, calculates Min value and adds Min value to vector
		if (percentMax < 0) {
			percentMin = (-1) * (avgFreqWindowAvg - actualFreqWindowAvg) / (avgFreqWindowAvg - minFreqWindowAvg) * 100;
			MinMaxValues.push_back(percentMin);
			
		} else MinMaxValues.push_back(percentMax);

	}
	return MinMaxValues;
}


// Stores MinMax frequency for each codon for the vector of sequences in an output file
void MinMax::outputFileMM(string file, vector< pair< string, vector<float> > > minMaxSequences) {

	string filename(file);
	filename = filename.erase(filename.size()-6, filename.size()); //".fasta"
//	cout << filename << endl;
	filename.append(".mm");	

	cout << "Creating " << filename << "..." << endl;

	ofstream ofile;
	ofile.open (filename.c_str());
	
	if (ofile.is_open()) {
	
		// For each sequence in vector, print description and vector of MinMax values
		// Separated by tabs
		for (int i = 0; i < minMaxSequences.size(); i++) {

			ofile << minMaxSequences[i].first;
//			ofile << endl;
			ofile << ",";

			for (int j = 0; j < minMaxSequences[i].second.size(); j++) {
			
				ofile << minMaxSequences[i].second[j];
//				ofile << ",";
				ofile << ",";
			}
			ofile << endl;
		}
		ofile.close();
		cout << filename << " has been created." << endl;
		
	} else cout << "Unable to open " << filename << endl;
}


// Transposes outputfile from rows to columns format for gnuplot & R downstream
void MinMax::transposeOutput(string file) {
	
	string tempName = file;
	tempName = tempName.erase(file.size()-3, file.size()); //".mm"
	tempName.append(".csv");
	string command = "./transpose -t --fsep ',' ";
	command.append(file);
	command.append(" > ");
	command.append(tempName);
	system(command.c_str());
	
	string renameFile = "mv ";
	renameFile.append(tempName);
	renameFile.append(" ");
	renameFile.append(file);
	system(renameFile.c_str());
}
