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
#include "CodonFrequency.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
//#include <regex>		// Regex to check for valid file ext WHY DOESN'T THE UNIV SUPPORT C++11?!
#include "stdlib.h"	// System command
#include <dirent.h>

using namespace std;

bool DirectoryExists(const char* pzPath) {
	if(pzPath == NULL) return false;
	
	DIR *pDir;
	bool bExists = false;
	
	pDir = opendir(pzPath);
	
	if(pDir != NULL) {
		bExists = true;
		(void) closedir (pDir);
	}
	return bExists;
}




int main() {

	vector <string> genomeFileNames;
	vector<ExtractSequence> genomeSeqs;
	string filename;
	bool filehasbeenread = false; // True only if a file has been read to make mm_plot function
	// Enter genome files
	cout << "- Enter .fasta file(s) to calculate codon frequency and min max values." << endl;
	cout << "- Enter an absolute path to a directory to read all .fasta files." << endl;
	cout << "- Enter .cf file(s) to skip codon frequency calculations and continue to min max." << endl;
	cout << "- After all files are inputed, leave the entry blank and press enter to continue." << endl;
	cout << "- 'q' to exit." << endl;
	
	// Loop to ask for file input
	while(1) {
		cout << ">";
		getline(cin, filename);
		
	  if(!filename.empty()) {
	  	ifstream file(filename.c_str());

			// Check if codon freq file (.fasta.txt) or .cf exists and readable
			if(file.good() && file.is_open()
			&& (filename.compare(filename.size()-10, 10,".fasta.txt") == 0
			|| filename.compare(filename.size()-3, 3, ".cf") == 0)) {
			
//				cout << "**CODON FREQ FILE." << endl;
				file.close();
				vector<Sequence> emptyVec;
				CodonFrequency cf(filename, emptyVec);
			
			
			// Check if .fasta file exists and readable
	  	} else if(file.good() && file.is_open()
	  	&& filename.compare(filename.size()-6, 6, ".fasta") == 0) {
	  	
//				cout << "**FASTA FILE" << endl;
	  		file.close();
	  		ExtractSequence Seqs(filename);				// Extract sequences from file
				genomeSeqs.push_back(Seqs);						// Adds sequences to vector of genomes
				genomeFileNames.push_back(filename);	// Adds genome file names to vector
				
				filehasbeenread = true;


			// Input directory to read all .fasta files
			} else if(DirectoryExists(filename.c_str())) {
					
					// Read all files in directory
					DIR *dpdf;
					struct dirent *epdf;
					dpdf = opendir(filename.c_str());
					if(dpdf != NULL) {
						while (epdf = readdir(dpdf)) {
						
							// Check for valid file extension
							string file = string(epdf->d_name);
							if(file.find(".fasta") != string::npos) {
							
								// Extract sequences from files, add seqs and file names to vectors
								ExtractSequence Seqs(file);
								genomeSeqs.push_back(Seqs);
								genomeFileNames.push_back(file);
//								cout << epdf->d_name << endl;
//								cout << file << endl;
								filehasbeenread = true;
							}
						}
					}
				
			  	if(filehasbeenread) {
  					break;
  				} else {
  					cout << "No file has been read. Enter another file or path directory." << endl;
		 		 	}
		
			// Exit program
			} else if(filename.compare("q") == 0) {
				exit(1);
		
			// Check if valid file extension
			} else if(filename.find(".fasta") == string::npos) {
				cout << "Invalid file format or directory path. Please enter another file name." << endl;
			
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
  cout << "-------------All files have been created-------------" << endl;
  system("mm_plot.py");
	return 0;
}
