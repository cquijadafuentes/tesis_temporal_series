#!/bin/bash

rm ./c_sim_temporal_series_mape
g++ -std=c++11 -g -O0 -DNDEBUG -fopenmp -o c_sim_temporal_series_mape c_sim_temporal_series_mape.cpp 

nohup ./c_sim_temporal_series_mape APCP_NLDAS_FORA0125_H.txt > ~/resultados_APC_sim_mape.txt &

nohup ./c_sim_temporal_series_mape CAPE_NLDAS_FORA0125_H.txt > ~/resultados_CAP_sim_mape.txt &

nohup ./c_sim_temporal_series_mape CONVfrac_NLDAS_FORA0125_H.txt > ~/resultados_CON_sim_mape.txt &

nohup ./c_sim_temporal_series_mape DLWRF_NLDAS_FORA0125_H.txt > ~/resultados_DLW_sim_mape.txt &

nohup ./c_sim_temporal_series_mape DSWRF_NLDAS_FORA0125_H.txt > ~/resultados_DSW_sim_mape.txt &

nohup ./c_sim_temporal_series_mape PEVAP_NLDAS_FORA0125_H.txt > ~/resultados_PEV_sim_mape.txt &

nohup ./c_sim_temporal_series_mape PRES_NLDAS_FORA0125_H.txt > ~/resultados_PRE_sim_mape.txt &

nohup ./c_sim_temporal_series_mape SPFH_NLDAS_FORA0125_H.txt > ~/resultados_SPF_sim_mape.txt &

nohup ./c_sim_temporal_series_mape TMP_NLDAS_FORA0125_H.txt > ~/resultados_TMP_sim_mape.txt &

nohup ./c_sim_temporal_series_mape UGRD_NLDAS_FORA0125_H.txt > ~/resultados_UGR_sim_mape.txt &

nohup ./c_sim_temporal_series_mape VGRD_NLDAS_FORA0125_H.txt > ~/resultados_VGR_sim_mape.txt &

