CPP=g++

PREFLAGS=-std=c++11 -g -O0 -DNDEBUG -I ~/include -L ~/lib

POSFLAGS=-lsdsl -ldivsufsort -ldivsufsort64

BINS=TSSM_build TSSM_build_full TSSM_histograma_build \
		TSSM_load_and_stats TSSM_load_and_preview \
		TSSMWEEK_build TSSMWEEK_load_compare \
		TSSMFACTOR_build

OBJECTS=TS_SM.o TS_SM_week.o TS_SM_factor.o

%.o: %.cpp
	$(CPP) $(PREFLAGS) -c $< -o $@ $(POSFLAGS)

default: bin

all: clean bin

compile: clean all

clean:
	rm -f $(BINS) $(OBJECTS)

bin: $(OBJECTS) $(BINS)

TSSM_build:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)

TSSM_build_full:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)

TSSM_histograma_build:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)

TSSM_load_and_stats:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)

TSSM_load_and_preview:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)

TSSMWEEK_build:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)

TSSMWEEK_load_compare:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)

TSSMFACTOR_build:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)
