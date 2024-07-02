#!/bin/bash

if [ $# -ne 1 ]; then
	echo "$(basename $0) <python_code>"
	echo "Correrá <python_code> con los listados de datos semanales y luego los completos."
	exit 1
fi

for i in APCP CAPE CONV DLWRF DSWRF PEVAP PRES SPFH TMP UGRD VGRD
do
	echo "Ejecutando: python3 ${1} ./listados_por_semana/${i}_NLDAS_week_01.txt"
	python3 ${1} ./listados_por_semana/${i}_NLDAS_week_01.txt
done

for i in APCP CAPE CONVfrac DLWRF DSWRF PEVAP PRES SPFH TMP UGRD VGRD
do
	echo "Ejecutando: python3 ${1} ${i}_NLDAS_FORA0125_H.txt"
	python3 ${1} ${i}_NLDAS_FORA0125_H.txt
done

