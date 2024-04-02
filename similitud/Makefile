CPP=g++

PREFLAGS=-std=c++11 -g -O0 -DNDEBUG -fopenmp

POSFLAGS=

BINS=c_distribucion_resultados\
		c_sim_espacial_moran_i_fullgrid\
		c_sim_temporal_series_euc\
		c_sim_temporal_series_mape\
		c_test_sim_temporal_euc\
		c_test_sim_temporal_euc_random

all: bin

bin: $(BINS)

c_distribucion_resultados:
	$(CPP) $(PREFLAGS) -o c_distribucion_resultados c_distribucion_resultados.cpp $(POSFLAGS)

c_sim_espacial_moran_i_fullgrid:
	$(CPP) $(PREFLAGS) -o c_sim_espacial_moran_i_fullgrid c_sim_espacial_moran_i_fullgrid.cpp $(POSFLAGS)

c_sim_temporal_series_euc:
	$(CPP) $(PREFLAGS) -o c_sim_temporal_series_euc c_sim_temporal_series_euc.cpp $(POSFLAGS)

c_test_sim_temporal_euc:
	$(CPP) $(PREFLAGS) -o c_test_sim_temporal_euc c_test_sim_temporal_euc.cpp $(POSFLAGS)

c_sim_temporal_series_mape:
	$(CPP) $(PREFLAGS) -o c_sim_temporal_series_mape c_sim_temporal_series_mape.cpp $(POSFLAGS)

c_test_sim_temporal_euc_random:
	$(CPP) $(PREFLAGS) -o c_test_sim_temporal_euc_random c_test_sim_temporal_euc_random.cpp $(POSFLAGS)
	
clean:
	rm -f $(BINS)