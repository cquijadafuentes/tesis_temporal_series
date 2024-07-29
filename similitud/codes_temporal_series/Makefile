CPP=g++

PREFLAGS=-std=c++11 -g -O0 -DNDEBUG -I ~/include -L ~/lib

POSFLAGS=-lsdsl -ldivsufsort -ldivsufsort64

BINS=ts_build_eeg_to_bin ts_build_nasa_to_bin \
		ts_build_sm_to_bin \
		ts_comp_eeg_to_bitcompress \
		ts_comp_nasa_to_bitcompress \
		ts_comp_nasa_to_encoder_segunanteriorvalor \
		ts_comp_nasa_to_k2tseries \
		ts_comp_nasa_to_k2tseries_sinfijas \
		ts_comp_sm_to_bitcompress \
		ts_test_read_bin_eeg \
		ts_test_read_bin_nasa ts_test_read_bin_sm

OBJECTS=

%.o: %.cpp
	$(CPP) $(PREFLAGS) -c $< -o $@ $(POSFLAGS)

all: bin

bin: $(OBJECTS) $(BINS)

ts_build_eeg_to_bin:
	$(CPP) -o $@ $@.cpp

ts_build_nasa_to_bin:
	$(CPP) -o $@ $@.cpp

ts_build_sm_to_bin:
	$(CPP) -o $@ $@.cpp

ts_comp_eeg_to_bitcompress:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(POSFLAGS)

ts_comp_nasa_to_bitcompress:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(POSFLAGS)

ts_comp_nasa_to_encoder_segunanteriorvalor:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(POSFLAGS)

ts_comp_nasa_to_k2tseries:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(POSFLAGS)

ts_comp_nasa_to_k2tseries_sinfijas:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(POSFLAGS)

ts_comp_sm_to_bitcompress:
	$(CPP) $(PREFLAGS) -o $@ $@.cpp $(POSFLAGS)

ts_test_read_bin_eeg:
	$(CPP) -o $@ $@.cpp

ts_test_read_bin_nasa:
	$(CPP) -o $@ $@.cpp

ts_test_read_bin_sm:
	$(CPP) -o $@ $@.cpp

clean:
	rm -f $(BINS) $(OBJECTS)

