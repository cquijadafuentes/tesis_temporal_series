CPP=g++

PREFLAGS=-std=c++11 -g -O0 -DNDEBUG -fopenmp

POSFLAGS=

BINS=c_distribucion_resultados\
		c_sim_temporal_series_euc\
		c_test_sim_temporal_euc

all: bin

bin: $(BINS)

c_distribucion_resultados:
	$(CPP) $(PREFLAGS) -o c_distribucion_resultados c_distribucion_resultados.cpp $(POSFLAGS)

c_sim_temporal_series_euc:
	$(CPP) $(PREFLAGS) -o c_sim_temporal_series_euc c_sim_temporal_series_euc.cpp $(POSFLAGS)

c_test_sim_temporal_euc:
	$(CPP) $(PREFLAGS) -o c_test_sim_temporal_euc c_test_sim_temporal_euc.cpp $(POSFLAGS)
	
clean:
	rm -f $(BINS)