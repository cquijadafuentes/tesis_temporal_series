#!/bin/bash

nohup python3 sim_diff_prom_celdas.py APCP_NLDAS_FORA0125_H.txt > ~/resultados_APC_diff_prom_celda.txt &

nohup python3 sim_diff_prom_celdas.py CAPE_NLDAS_FORA0125_H.txt > ~/resultados_CAP_diff_prom_celda.txt &

nohup python3 sim_diff_prom_celdas.py CONVfrac_NLDAS_FORA0125_H.txt > ~/resultados_CON_diff_prom_celda.txt &

nohup python3 sim_diff_prom_celdas.py DLWRF_NLDAS_FORA0125_H.txt > ~/resultados_DLW_diff_prom_celda.txt &

nohup python3 sim_diff_prom_celdas.py DSWRF_NLDAS_FORA0125_H.txt > ~/resultados_DSW_diff_prom_celda.txt &

nohup python3 sim_diff_prom_celdas.py PEVAP_NLDAS_FORA0125_H.txt > ~/resultados_PEV_diff_prom_celda.txt &

nohup python3 sim_diff_prom_celdas.py PRES_NLDAS_FORA0125_H.txt > ~/resultados_PRE_diff_prom_celda.txt &

nohup python3 sim_diff_prom_celdas.py SPFH_NLDAS_FORA0125_H.txt > ~/resultados_SPF_diff_prom_celda.txt &

nohup python3 sim_diff_prom_celdas.py TMP_NLDAS_FORA0125_H.txt > ~/resultados_TMP_diff_prom_celda.txt &

nohup python3 sim_diff_prom_celdas.py UGRD_NLDAS_FORA0125_H.txt > ~/resultados_UGR_diff_prom_celda.txt &

nohup python3 sim_diff_prom_celdas.py VGRD_NLDAS_FORA0125_H.txt > ~/resultados_VGR_diff_prom_celda.txt &

