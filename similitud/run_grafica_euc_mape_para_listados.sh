#!/bin/bash

for i in APCP CAPE CONV DLWRF DSWFR PEVAP PRES SPFH TMP UGRD VGRD
do
	echo "\tgraficando datos"
	python3 ../../sim_promedio_celdas.py input_test_${i}_0101.txt
	python3 ../../sim_promedio_celdas.py input_test_${i}_0315.txt
	echo "\tobteniendo similitudes por celda"
	../../c_sim_temporal_series_euc input_test_${i}.txt > salida_similitud_euclidea_${i}.txt
	../../c_sim_temporal_series_mape input_test_${i}.txt > salida_similitud_mape_${i}.txt
	echo "\tobteniendo distribución de similitudes por celda"
	../../c_distribucion_resultados salida_similitud_euclidea_${i}.txt > toPlot_distribucion_similitud_euclidea_${i}.txt
	../../c_distribucion_resultados salida_similitud_mape_${i}.txt > toPlot_distribucion_similitud_mape_${i}.txt
	cd ..
done

echo "FIN"