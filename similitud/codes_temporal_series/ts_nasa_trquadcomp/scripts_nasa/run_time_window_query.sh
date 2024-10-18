#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Usage: $(basename $0) <repeticiones> <output_file>"
	exit 1
fi

REPETICIONES=$1
OUTPUTFILE=$2
EXEC="/home/cquijada/datasets/time_series/temporal/NASA/NLDAS_FORA0125_H/quadcomp/codes/TRQC_time_windowQuery"
DATAFOLDER="/home/cquijada/datasets/time_series/temporal/NASA/NLDAS_FORA0125_H/quadcomp/data/"
QUERYFILE="/home/cquijada/datasets/time_series/temporal/NASA/NLDAS_FORA0125_H/quadcomp/queries/query_window_fromregionquery_10000.txt"

echo "Instrucción:" > ${OUTPUTFILE}
echo "$0 $1 $2" >> ${OUTPUTFILE}
echo "----------------------------------------------------------------" >> ${OUTPUTFILE}
echo "Fecha INICIO ejecución: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTPUTFILE}
echo "Directorio de ejecución: " >> ${OUTPUTFILE}
pwd >> ${OUTPUTFILE}
echo "" >> ${OUTPUTFILE}
echo "******************** WINDOW QUERY QUADCOMP ********************" >> ${OUTPUTFILE}
echo "" >> ${OUTPUTFILE}
echo "Archivo de ejecución quadcomp: ${EXEC}" >> ${OUTPUTFILE}
echo "Directorio de datos: ${DATAFOLDER}" >> ${OUTPUTFILE}
echo "Archivo de queries: ${QUERYFILE}" >> ${OUTPUTFILE}
echo "" >> ${OUTPUTFILE}

for filename in APCP CAPE CONVfrac DLWRF DSWRF PEVAP PRES SPFH TMP UGRD VGRD
do
	${EXEC} ${DATAFOLDER}${filename}.qc04 ${QUERYFILE} ${REPETICIONES} >> ${OUTPUTFILE}
done

echo "" >> ${OUTPUTFILE}
echo "******************** WINDOW QUERY TK2-RASTER ********************" >> ${OUTPUTFILE}
echo "" >> ${OUTPUTFILE}

EXEC="/home/cquijada/k2-raster/build/bin/get_values_window_tk2r"
DATAFOLDER="/home/cquijada/datasets/time_series/temporal/NASA/NLDAS_FORA0125_H/tk2r_files_110/"

echo "Archivo de ejecución tk2-raster: ${EXEC}" >> ${OUTPUTFILE}
echo "Directorio de datos: ${DATAFOLDER}" >> ${OUTPUTFILE}
echo "Archivo de queries: ${QUERYFILE}" >> ${OUTPUTFILE}
echo "" >> ${OUTPUTFILE}

for filename in APCP CAPE CONVfrac DLWRF DSWRF PEVAP PRES SPFH TMP UGRD VGRD
do
	echo "${filename}_tk2r_110.tk2r" >> ${OUTPUTFILE}
	${EXEC} ${DATAFOLDER}${filename}_tk2r_110.tk2r ${QUERYFILE} 0 ${REPETICIONES} >> ${OUTPUTFILE}
done

echo "" >> ${OUTPUTFILE}
echo "----------------------------------------------------------------" >> ${OUTPUTFILE}
echo "" >> ${OUTPUTFILE}

echo "Fecha FIN ejecución: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTPUTFILE}
exit 0

