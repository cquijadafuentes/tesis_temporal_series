#!/bin/bash

if [ $# -ne 3 ]; then
	echo "$(basename $0) <EEG_filelist.txt> <executable_BIN_build> <output_file>"
	exit 1
fi

LISTFILE=$1
BUILDBIN=$2
OUTFILE=$3

echo "Fecha INICIO EEG BIN variante: $(date +'%Y/%m/%d %H:%M:%S')" > ${OUTFILE}
echo "Ejecutando ${BUILDBIN}" >> ${OUTFILE}

while read p; do
	./${BUILDBIN} ${p}.qc4 >> ${OUTFILE}
done < "${LISTFILE}"

echo "Fecha FIN EEG BIN variante: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTFILE}

