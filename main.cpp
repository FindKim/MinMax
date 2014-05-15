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

using namespace std;

int main() {
  
// testing to verify the frequency calculations
  ExtractSequence codFreq("Ypes.fasta");
  MinMax calcMinMax("Ypes.fasta", codFreq.getVectorOfSequences());
	return 0;
}
