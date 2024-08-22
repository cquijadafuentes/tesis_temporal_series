CPP=g++

PREFLAGS=-std=c++11 -g -O0 -DNDEBUG -I ~/include -L ~/lib

POSFLAGS=-lsdsl -ldivsufsort -ldivsufsort64

BINS=NQC_build_from_bin

OBJECTS=NASA_QuadComp.o

%.o: %.cpp
	$(CPP) $(PREFLAGS) -c $< -o $@ $(POSFLAGS)

all: bin

bin: $(OBJECTS) $(BINS)
	
NQC_build_from_bin:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)

clean:
	rm -f $(BINS) $(OBJECTS)


