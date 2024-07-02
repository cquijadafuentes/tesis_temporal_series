#!/bin/bash

if [ $# -ne 2 ]; then
	echo "$(basename $0) <python_code> <output_file>"
	echo "Correrá <python_code> con los listados de datos semanales y luego los completos."
	echo "Y la salida se guardará en el <output_file>"
	exit 1
fi

echo "Iniciado en: $(date +'%Y/%m/%d %H:%M:%S')" > ${2}

for i in APCP CAPE CONV DLWRF DSWRF PEVAP PRES SPFH TMP UGRD VGRD
do
	echo "Ejecutando: python3 ${1} ./listados_por_semana/${i}_NLDAS_week_01.txt"
	python3 ${1} ./listados_por_semana/${i}_NLDAS_week_01.txt >> ${2}
done

for i in APCP CAPE CONVfrac DLWRF DSWRF PEVAP PRES SPFH TMP UGRD VGRD
do
	echo "Ejecutando: python3 ${1} ${i}_NLDAS_FORA0125_H.txt"
	python3 ${1} ${i}_NLDAS_FORA0125_H.txt >> ${2}
done

echo "Finalizado en: $(date +'%Y/%m/%d %H:%M:%S')" >> ${2}
