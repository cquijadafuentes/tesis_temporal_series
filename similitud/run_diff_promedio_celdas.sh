#!/bin/bash

nohup python3 sim_diff_prom_celdas.py APCP_NLDAS_FORA0125_H.txt > ~/resultados_APC_promedio_celda.txt &

nohup python3 sim_diff_prom_celdas.py CAPE_NLDAS_FORA0125_H.txt > ~/resultados_CAP_promedio_celda.txt &

nohup python3 sim_diff_prom_celdas.py CONVfrac_NLDAS_FORA0125_H.txt > ~/resultados_CON_promedio_celda.txt &

nohup python3 sim_diff_prom_celdas.py DLWRF_NLDAS_FORA0125_H.txt > ~/resultados_DLW_promedio_celda.txt &

nohup python3 sim_diff_prom_celdas.py DSWRF_NLDAS_FORA0125_H.txt > ~/resultados_DSW_promedio_celda.txt &

nohup python3 sim_diff_prom_celdas.py PEVAP_NLDAS_FORA0125_H.txt > ~/resultados_PEV_promedio_celda.txt &

nohup python3 sim_diff_prom_celdas.py PRES_NLDAS_FORA0125_H.txt > ~/resultados_PRE_promedio_celda.txt &

nohup python3 sim_diff_prom_celdas.py SPFH_NLDAS_FORA0125_H.txt > ~/resultados_SPF_promedio_celda.txt &

nohup python3 sim_diff_prom_celdas.py TMP_NLDAS_FORA0125_H.txt > ~/resultados_TMP_promedio_celda.txt &

nohup python3 sim_diff_prom_celdas.py UGRD_NLDAS_FORA0125_H.txt > ~/resultados_UGR_promedio_celda.txt &

nohup python3 sim_diff_prom_celdas.py VGRD_NLDAS_FORA0125_H.txt > ~/resultados_VGR_promedio_celda.txt &

