all: main

main: main.o Sequence.o ExtractSequence.o CodonFrequency.o MinMax.o Domain.o
	g++ main.o Sequence.o ExtractSequence.o CodonFrequency.o MinMax.o Domain.o -o main

main.o: main.cpp
	g++ -c main.cpp

Sequence.o: Sequence.cpp Sequence.h
	g++ -c Sequence.cpp

Domain.o: Domain.cpp Domain.h
	g++ -c Domain.cpp

ExtractSequence.o: ExtractSequence.cpp ExtractSequence.h
	g++ -c ExtractSequence.cpp

CodonFrequency.o: CodonFrequency.cpp CodonFrequency.h
	g++ -g -c CodonFrequency.cpp

MinMax.o: MinMax.cpp MinMax.h
	g++ -c MinMax.cpp

clean:
	rm -f *.o *.mm *.cf *.ps *~ main
