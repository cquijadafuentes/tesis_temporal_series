#!/bin/bash

if [ $# -ne 3 ]; then
	echo "$(basename $0) <executable_build_nasa> <destination_folder> <output_file>"
	exit 1
fi

FILES="APCP CAPE CONVfrac DLWRF DSWRF PEVAP PRES SPFH TMP UGRD VGRD"
SUFIX="_NLDAS_FORA0125_H"
EXEC=$1
FOLDER=$2
OUTFILE=$3

echo "Fecha INICIO BUILD: $(date +'%Y/%m/%d %H:%M:%S')" > ${OUTFILE}

for file in ${FILES}
do
	${EXEC} ${file}${SUFIX}.txt ${FOLDER}${file}${SUFIX}.bin >> ${OUTFILE}
done

echo "Fecha FIN BUILD: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTFILE}


