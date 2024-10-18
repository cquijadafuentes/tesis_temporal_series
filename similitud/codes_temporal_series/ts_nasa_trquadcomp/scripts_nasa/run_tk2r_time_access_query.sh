#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Usage: $(basename $0) <repeticiones> <output_file>"
	exit 1
fi

REPETICIONES=$1
OUTPUTFILE=$2
EXEC="/home/cquijada/k2-raster/build/bin/get_cell_tk2r"
DATAFOLDER="/home/cquijada/datasets/time_series/temporal/NASA/NLDAS_FORA0125_H/tk2r_files_110/"
QUERYFILE="/home/cquijada/datasets/time_series/temporal/NASA/NLDAS_FORA0125_H/quadcomp/queries/query_access_10k.txt"

echo "Instrucción:" >> ${OUTPUTFILE}
echo "$0 $1 $2" >> ${OUTPUTFILE}
echo "Directorio de inicio: " >> ${OUTPUTFILE}
pwd >> ${OUTPUTFILE}
echo "ACCESS QUERY" >> ${OUTPUTFILE}
echo "Fecha INICIO ejecución: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTPUTFILE}
echo "" >> ${OUTPUTFILE}

for filename in APCP CAPE CONVfrac DLWRF DSWRF PEVAP PRES SPFH TMP UGRD VGRD
do
	${EXEC} ${DATAFOLDER}${filename}_tk2r_110.tk2r ${QUERYFILE} ${REPETICIONES} >> ${OUTPUTFILE}
done

echo "Fecha FIN ejecución: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTPUTFILE}
exit 0

