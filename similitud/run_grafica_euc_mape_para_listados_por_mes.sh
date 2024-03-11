#!/bin/bash

OUTPUT_FOLDER="./listados_por_mes/resultados_graficas/"

for i in APCP CAPE CONV DLWRF DSWFR PEVAP PRES SPFH TMP UGRD VGRD
do
	for m in '01' '02' '03' '04'
	do
		FILENAME="${i}_NLDAS_month_${m}"
		INPUT_FILE="./listados_por_hora/${FILENAME}.txt"
		echo "\tgraficando datos ${i}_NLDAS_month_${m}.txt"
		python3 sim_promedio_celdas.py ${INPUT_FILE}
		echo "\tobteniendo similitudes por celda"
		./c_sim_temporal_series_euc ${INPUT_FILE} > ${OUTPUT_FOLDER}${FILENAME}_valores_sim_euc.txt
		./c_sim_temporal_series_mape ${INPUT_FILE} > ${OUTPUT_FOLDER}${FILENAME}_valores_sim_mape.txt
		echo "\tobteniendo distribución de similitudes por celda"
		./c_distribucion_resultados ${OUTPUT_FOLDER}${FILENAME}_valores_sim_euc.txt > ${OUTPUT_FOLDER}${FILENAME}_distrib_sim_euc.txt
		./c_distribucion_resultados ${OUTPUT_FOLDER}${FILENAME}_valores_sim_mape.txt > ${OUTPUT_FOLDER}${FILENAME}_distrib_sim_mape.txt
	done
done

echo "FIN"