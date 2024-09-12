CPP=g++

PREFLAGS=-std=c++11 -g -O0 -DNDEBUG -I ~/include -L ~/lib

POSFLAGS=-lsdsl -ldivsufsort -ldivsufsort64

BINS=TRQC_build_from_bin TRQC_load_trqc \
	TRQC_test_bin_trqc TRQC_test_bin_trqc_windowQuery \
	TRQC_test_fromInput \
	TRQC_test_windowQ_fromInput 

OBJECTS=TempRaster_QuadComp.o

%.o: %.cpp
	$(CPP) $(PREFLAGS) -c $< -o $@ $(POSFLAGS)

all: bin

bin: $(OBJECTS) $(BINS)
	
TRQC_build_from_bin:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)
	
TRQC_load_trqc:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)

TRQC_test_bin_trqc:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)

TRQC_test_fromInput:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)

TRQC_test_windowQ_fromInput:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)

TRQC_test_bin_trqc_windowQuery:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)

clean:
	rm -f $(BINS) $(OBJECTS)

compile: clean all
