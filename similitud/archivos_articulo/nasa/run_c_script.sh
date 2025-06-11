#!/bin/bash

if [ $# -ne 2 ]; then
	echo "$(basename $0) <c_code> <outfile>"
	echo "Correrá <c_code> con archivos NASA."
	echo "SE DEBE CORRER EN EL DIRECTORIO DE LOS ARCHIVOS DE DATOS!"
	exit 1
fi

echo "Iniciado en: $(date +'%Y/%m/%d %H:%M:%S')" > ${2}
echo "Corriendo ${0} ${1} ${2}" >> ${2}


for i in APCP CAPE CONVfrac DLWRF DSWRF PEVAP PRES SPFH TMP UGRD VGRD
do
	echo " +++++++++ Ejecutando: ${1} ${i}_NLDAS_FORA0125_H.txt" >> ${2}
	${1} ${i}_NLDAS_FORA0125_H.txt >> ${2}
done

echo "Finalizado en: $(date +'%Y/%m/%d %H:%M:%S')" >> ${2}

