#!/bin/bash

nohup ./c_sim_temporal_series_euc APCP_NLDAS_FORA0125_H.txt > ~/resultados_APC_sim_euclidea.txt &

nohup ./c_sim_temporal_series_euc CAPE_NLDAS_FORA0125_H.txt > ~/resultados_CAP_sim_euclidea.txt &

nohup ./c_sim_temporal_series_euc CONVfrac_NLDAS_FORA0125_H.txt > ~/resultados_CON_sim_euclidea.txt &

nohup ./c_sim_temporal_series_euc DLWRF_NLDAS_FORA0125_H.txt > ~/resultados_DLW_sim_euclidea.txt &

nohup ./c_sim_temporal_series_euc DSWRF_NLDAS_FORA0125_H.txt > ~/resultados_DSW_sim_euclidea.txt &

nohup ./c_sim_temporal_series_euc PEVAP_NLDAS_FORA0125_H.txt > ~/resultados_PEV_sim_euclidea.txt &

nohup ./c_sim_temporal_series_euc PRES_NLDAS_FORA0125_H.txt > ~/resultados_PRE_sim_euclidea.txt &

nohup ./c_sim_temporal_series_euc SPFH_NLDAS_FORA0125_H.txt > ~/resultados_SPF_sim_euclidea.txt &

nohup ./c_sim_temporal_series_euc TMP_NLDAS_FORA0125_H.txt > ~/resultados_TMP_sim_euclidea.txt &

nohup ./c_sim_temporal_series_euc UGRD_NLDAS_FORA0125_H.txt > ~/resultados_UGR_sim_euclidea.txt &

nohup ./c_sim_temporal_series_euc VGRD_NLDAS_FORA0125_H.txt > ~/resultados_VGR_sim_euclidea.txt &

