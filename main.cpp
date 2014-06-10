//
//  Main.cpp
//  
//	Driver program for Sequence class
//	Reads file; parses file; prints name, description, and sequence
//
//  Created by Kim Ngo on 14-3-15.
//
//

#include "Sequence.h"
#include "ExtractSequence.h"
#include "MinMax.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main() {

//	vector< pair<string, pair<ExtractSequence, CodonFrequency> > >;
	vector <char *> genomeFileNames;
	vector<ExtractSequence> genomeSeqs;
	string filename = "empty";

	// Enter genome files
	cout << "Enter genome file names to calculate codon frequency or press enter to continue: ";
	while(!filename.empty()) {
  	cin >> filename;
	  if(!filename.empty()) {
			char *file = &filename[0];
			ExtractSequence Seqs(file);				// Extract sequences from file
			genomeSeqs.push_back(Seqs);				// Adds sequences to vector of genomes
			genomeFileNames.push_back(file);	// Adds genome file names to vector
  	}
	}
	
	vector<char *>::iterator genomes_it = genomeFileNames.begin();
	for(int genomeSeqs_it = 0; genomes_it != genomeFileNames.end(); genomes_it++, genomeSeqs_it++) {
	  CodonFrequency CF(*genomes_it, genomeSeqs[genomeSeqs_it].getVectorOfSequences());
  	MinMax calcMinMax(*genomes_it, genomeSeqs[genomeSeqs_it].getVectorOfSequences(), CF);
  }
  
	return 0;
}
