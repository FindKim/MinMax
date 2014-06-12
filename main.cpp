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

	vector <string> genomeFileNames;
	vector<ExtractSequence> genomeSeqs;
	string filename;
	// Enter genome files
	cout << "- Enter file names to calculate codon frequency and min max values for a list of genomes." << endl;
	cout << "- After all intended genomes are inputed, leave the entry blank and press enter to continue." << endl;
	
	// Loop to ask for file input
	while(1) {
	
		getline(cin, filename);
	  if(!filename.empty()) {

	  	ifstream file(filename.c_str());

			// Check if file exists and readable
	  	if(file.good() && file.is_open()) {
	  		file.close();
	  		cout << file << endl;
	  		ExtractSequence Seqs(filename);				// Extract sequences from file
				genomeSeqs.push_back(Seqs);						// Adds sequences to vector of genomes
				genomeFileNames.push_back(filename);	// Adds genome file names to vector
			
			// Error message for non-existing files
	  	} else if(!file.eof()) {
	  		cout << "Error openning '" << filename << "'. Please enter another file name.\n";				
			}

		// Exits loop--stops asking for file input
		} else if(filename.empty()){
				break;
  	}
	}

	// Calculates minmax (.mm) and codon freq (.cf) for each file input
	vector<string>::iterator genomes_it = genomeFileNames.begin();
	for(int genomeSeqs_it = 0; genomes_it != genomeFileNames.end(); genomes_it++, genomeSeqs_it++) {
//			cout << "'" << *genomes_it << "'" << endl;
	  CodonFrequency CF(*genomes_it, genomeSeqs[genomeSeqs_it].getVectorOfSequences());
  	MinMax calcMinMax(*genomes_it, genomeSeqs[genomeSeqs_it].getVectorOfSequences(), CF);
  }
  
	return 0;
}
