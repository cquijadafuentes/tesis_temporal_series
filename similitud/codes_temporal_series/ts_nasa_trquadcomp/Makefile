CPP=g++

PREFLAGS=-std=c++11 -g -O0 -DNDEBUG -I ~/include -L ~/lib

POSFLAGS=-lsdsl -ldivsufsort -ldivsufsort64

BINS=TRQC_eeg_build_from_bin \
		TRQC_eeg_build_variante_columnas \
		TRQC_eeg_build_variante_filas \
		TRQC_eeg_time_getseries_queries \
		TRQC_load_trqc \
		TRQC_nasa_build_from_bin \
		TRQC_print_getSerie \
		TRQC_test_eeg_bin_trqc \
		TRQC_test_nasa_bin_trqc \
		TRQC_test_nasa_bin_trqc_windowQuery \
		TRQC_test_nasa_fromInput \
		TRQC_test_nasa_windowQ_fromInput \
		TRQC_time_accessQuery \
		TRQC_time_getSerieQuery \
		TRQC_time_windowQuery \
		TRQC_to_bin_tk2tree_format \
		TRQC_user_menu

OBJECTS=TempRaster_QuadComp.o

%.o: %.cpp
	$(CPP) $(PREFLAGS) -c $< -o $@ $(POSFLAGS)

all: bin

bin: $(OBJECTS) $(BINS)
	
TRQC_eeg_build_from_bin:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)
	
TRQC_eeg_build_variante_filas:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)
	
TRQC_eeg_build_variante_columnas:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)
	
TRQC_eeg_time_getseries_queries:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)
	
TRQC_load_trqc:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)

TRQC_nasa_build_from_bin:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)

TRQC_print_getSerie:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)

TRQC_test_eeg_bin_trqc:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)

TRQC_test_nasa_bin_trqc:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)

TRQC_test_nasa_fromInput:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)

TRQC_test_nasa_windowQ_fromInput:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)

TRQC_test_nasa_bin_trqc_windowQuery:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)

TRQC_time_accessQuery:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)

TRQC_time_getSerieQuery:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)

TRQC_time_windowQuery:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)

TRQC_to_bin_tk2tree_format:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)

TRQC_user_menu:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(OBJECTS) $(POSFLAGS)

clean:
	rm -f $(BINS) $(OBJECTS)

compile: clean all
