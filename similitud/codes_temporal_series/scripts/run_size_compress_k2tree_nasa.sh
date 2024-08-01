#!/bin/bash

if [ $# -ne 2 ]; then
	echo "$(basename $0) <executable_size_nasa> <output_file>"
	exit 1
fi

FILES="APCP CAPE CONVfrac DLWRF DSWRF PEVAP PRES SPFH TMP UGRD VGRD"
SIZES="4 8 12 25 50 100"
SUFIX="_NLDAS_FORA0125_H"
EXEC=$1
OUTFILE=$2

echo "Fecha INICIO SIZE COMPRESS: $(date +'%Y/%m/%d %H:%M:%S')" > ${OUTFILE}
for s in ${SIZES}
do
	for file in ${FILES}
	do
		echo "${EXEC} ${file}${SUFIX}.bin ${s} >> ${OUTFILE}"
	done
done

echo "Fecha FIN SIZE COMPRESS: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTFILE}

