#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Usage: $(basename $0) <repeticiones> <output_file>"
	exit 1
fi

REPETICIONES=$1
OUTPUTFILE=$2
MAINFOLDER="/home/cquijada/datasets/time_series/temporal/NASA/NLDAS_FORA0125_H/quadcomp"
EXEC="${MAINFOLDER}/codes/TRQC_time_windowQuery"
DATAFOLDER="${MAINFOLDER}/data/"
QUERYFILE="${MAINFOLDER}/queries/query_window_10k.txt"

echo "Fecha INICIO ejecución: $(date +'%Y/%m/%d %H:%M:%S')" > ${OUTPUTFILE}
echo "Directorio de inicio: " >> ${OUTPUTFILE}
pwd >> ${OUTPUTFILE}
echo "WINDOW QUERY" >> ${OUTPUTFILE}
echo "" >> ${OUTPUTFILE}

for filename in APCP CAPE CONVfrac DLWRF DSWRF PEVAP PRES SPFH TMP UGRD VGRD
do
	${EXEC} ${DATAFOLDER}${filename}.qc04 ${QUERYFILE} ${REPETICIONES} >> ${OUTPUTFILE}
done

echo "Fecha FIN ejecución: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTPUTFILE}
exit 0

