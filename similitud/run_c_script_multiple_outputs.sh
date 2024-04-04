#!/bin/bash

if [ $# -ne 2 ]; then
	echo "$(basename $0) <c_code> <prefix_outfiles>"
	echo "Correrá <c_code> con los listados de datos semanales y luego los completos."
	echo "Y los resultados se guardan en archivos con que inician su nombre con <prefix_outfiles>"
	exit 1
fi

echo "Iniciado en: $(date +'%Y/%m/%d %H:%M:%S')"

for i in APCP CAPE CONV DLWRF DSWRF PEVAP PRES SPFH TMP UGRD VGRD
do
	echo " +++++++++ Ejecutando: ./${1} ./listados_por_semana/${i}_NLDAS_week_01.txt"
	./${1} ./listados_por_semana/${i}_NLDAS_week_01.txt > ${2}.week_01.${i}.data
done

for i in APCP CAPE CONVfrac DLWRF DSWRF PEVAP PRES SPFH TMP UGRD VGRD
do
	echo " +++++++++ Ejecutando: ./${1} ${i}_NLDAS_FORA0125_H.txt"
	./${1} ${i}_NLDAS_FORA0125_H.txt  > ${2}.full.${i}.data
done

echo "Finalizado en: $(date +'%Y/%m/%d %H:%M:%S')"
