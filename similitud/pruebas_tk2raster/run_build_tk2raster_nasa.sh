#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Usage: $(basename $0) <output_file>"
	exit 1
fi

OUTPUTFILE=$1

BTK2R="~/k2-raster/build/bin/encode_tk2r"
echo "Iniciado en: $(date +'%Y/%m/%d %H:%M:%S')" > ${OUTPUTFILE}

for i in APCP CAPE CONVfrac DLWRF DSWRF PEVAP PRES SPFH TMP UGRD VGRD
do
	echo "${BTK2R} ${i}_NLDAS_FORA0125_H.txt ./ ./tk2r_new/${i}.tk2r -f 6 -s 2" >> ${OUTPUTFILE}
	${BTK2R} ${i}_NLDAS_FORA0125_H.txt ./ ./tk2r_new/${i}.tk2r -f 6 -s 2 >> ${OUTPUTFILE}
	echo " " >> ${OUTPUTFILE}
done

echo "Finalizado en: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTPUTFILE}