all: minmax transpose

minmax: main.o Sequence.o ExtractSequence.o CodonFrequency.o MinMax.o
	g++ main.o Sequence.o ExtractSequence.o CodonFrequency.o MinMax.o -o minmax

main.o: main.cpp
	g++ -c main.cpp

Sequence.o: Sequence.cpp Sequence.h
	g++ -c Sequence.cpp

ExtractSequence.o: ExtractSequence.cpp ExtractSequence.h
	g++ -c ExtractSequence.cpp

CodonFrequency.o: CodonFrequency.cpp CodonFrequency.h
	g++ -g -c CodonFrequency.cpp

MinMax.o: MinMax.cpp MinMax.h
	g++ -c MinMax.cpp

transpose: transpose.c
	gcc transpose.c -o transpose
	# Compiles transpose supporting c prog that transposes .mm file from rows to columns

clean:
	rm -f *.o *.pyc *~
