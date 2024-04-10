CPP=g++

PREFLAGS=-std=c++11 -g -O0 -DNDEBUG -fopenmp

POSFLAGS=

BINS=c_distribucion_resultados\
		c_sim_espacial_moran_i_fullgrid\
		c_sim_espacial_moran_i_reina_orden_1\
		c_sim_espacial_moran_i_reina_orden_1_media\
		c_sim_serietemporal_moran_i_reina_orden_1_distEucST\
		c_sim_serietemporal_moran_i_reina_orden_1_mediaST\
		c_sim_serietemporal_moran_i_reina_orden_1_sumadiffST\
		c_sim_serietemporal_moran_i_reina_orden_2_distEucST\
		c_sim_temporal_series_euc\
		c_sim_temporal_series_mape\
		c_test_moran_i\
		c_test_printFirstRow_t0\
		c_test_sim_temporal_euc\
		c_test_sim_temporal_euc_random

all: bin

bin: $(BINS)

c_distribucion_resultados:
	$(CPP) $(PREFLAGS) -o c_distribucion_resultados c_distribucion_resultados.cpp $(POSFLAGS)

c_sim_espacial_moran_i_fullgrid:
	$(CPP) $(PREFLAGS) -o c_sim_espacial_moran_i_fullgrid c_sim_espacial_moran_i_fullgrid.cpp $(POSFLAGS)

c_sim_espacial_moran_i_reina_orden_1:
	$(CPP) $(PREFLAGS) -o c_sim_espacial_moran_i_reina_orden_1 c_sim_espacial_moran_i_reina_orden_1.cpp $(POSFLAGS)

c_sim_espacial_moran_i_reina_orden_1_media:
	$(CPP) $(PREFLAGS) -o c_sim_espacial_moran_i_reina_orden_1_media c_sim_espacial_moran_i_reina_orden_1_media.cpp $(POSFLAGS)

c_sim_serietemporal_moran_i_reina_orden_1_distEucST:
	$(CPP) $(PREFLAGS) -o c_sim_serietemporal_moran_i_reina_orden_1_distEucST c_sim_serietemporal_moran_i_reina_orden_1_distEucST.cpp $(POSFLAGS)

c_sim_serietemporal_moran_i_reina_orden_1_mediaST:
	$(CPP) $(PREFLAGS) -o c_sim_serietemporal_moran_i_reina_orden_1_mediaST c_sim_serietemporal_moran_i_reina_orden_1_mediaST.cpp $(POSFLAGS)

c_sim_serietemporal_moran_i_reina_orden_1_sumadiffST:
	$(CPP) $(PREFLAGS) -o c_sim_serietemporal_moran_i_reina_orden_1_sumadiffST c_sim_serietemporal_moran_i_reina_orden_1_sumadiffST.cpp $(POSFLAGS)

c_sim_serietemporal_moran_i_reina_orden_2_distEucST:
	$(CPP) $(PREFLAGS) -o c_sim_serietemporal_moran_i_reina_orden_2_distEucST c_sim_serietemporal_moran_i_reina_orden_2_distEucST.cpp $(POSFLAGS)

c_sim_temporal_series_euc:
	$(CPP) $(PREFLAGS) -o c_sim_temporal_series_euc c_sim_temporal_series_euc.cpp $(POSFLAGS)

c_test_moran_i:
	$(CPP) $(PREFLAGS) -o c_test_moran_i c_test_moran_i.cpp $(POSFLAGS)

c_test_printFirstRow_t0:
	$(CPP) $(PREFLAGS) -o c_test_printFirstRow_t0 c_test_printFirstRow_t0.cpp $(POSFLAGS)

c_test_sim_temporal_euc:
	$(CPP) $(PREFLAGS) -o c_test_sim_temporal_euc c_test_sim_temporal_euc.cpp $(POSFLAGS)

c_sim_temporal_series_mape:
	$(CPP) $(PREFLAGS) -o c_sim_temporal_series_mape c_sim_temporal_series_mape.cpp $(POSFLAGS)

c_test_sim_temporal_euc_random:
	$(CPP) $(PREFLAGS) -o c_test_sim_temporal_euc_random c_test_sim_temporal_euc_random.cpp $(POSFLAGS)
	
clean:
	rm -f $(BINS)