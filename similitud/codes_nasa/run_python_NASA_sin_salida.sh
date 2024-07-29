#!/bin/bash

if [ $# -ne 1 ]; then
	echo "$(basename $0) <python_code>"
	echo "Correrá <python_code> con los listados de datos semanales y luego los completos."
	exit 1
fi

for i in APCP CAPE CONVfrac DLWRF DSWRF PEVAP PRES SPFH TMP UGRD VGRD
do
	echo "Ejecutando: python3 ${1} ${i}_NLDAS_FORA0125_H.txt"
	python3 ${1} ${i}_NLDAS_FORA0125_H.txt
done

