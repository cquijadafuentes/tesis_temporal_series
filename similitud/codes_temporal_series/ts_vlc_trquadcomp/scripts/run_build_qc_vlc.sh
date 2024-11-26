#!/bin/bash

if [ $# -ne 4 ]; then
	echo "$(basename $0) <EEG_filelist.txt> <executable_BIN_build> <mapping_list> <output_file>"
	exit 1
fi


LISTFILE=$1
BUILDBIN=$2
MAPLIST=$3
OUTFILE=$4

echo "Fecha INICIO FIN BUILD EEG VLC: $(date +'%Y/%m/%d %H:%M:%S')" > ${OUTFILE}
./${BUILDBIN} >> ${OUTFILE}
echo "Ejecutando $0 ${LISTFILE} ${BUILDBIN} ${MAPLIST} ${OUTFILE}" >> ${OUTFILE}
while read p; do
	./${BUILDBIN} ${p}.bin ${MAPLIST} 4 ${p}.qc4vlc >> ${OUTFILE}
done < "${LISTFILE}"

echo "Fecha FIN BUILD EEG VLC: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTFILE}

