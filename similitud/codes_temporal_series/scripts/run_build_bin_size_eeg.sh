#!/bin/bash

if [ $# -ne 4 ]; then
	echo "$(basename $0) <filelist.txt> <executable_build> <executable_size_comp> <output_file>"
	exit 1
fi

LISTFILE=$1
EXECBUILD=$2
EXECSIZECOMP=$3
OUTFILE=$4

echo "Fecha INICIO SIZE COMPRESS: $(date +'%Y/%m/%d %H:%M:%S')" > ${OUTFILE}

while read p; do
  echo "Procesando ${p}"
  ./${EXECBUILD} ${p}_float.txt ${p}.eeg.bin
  ./${EXECSIZECOMP} ${p}.eeg.bin >> ${OUTFILE}

done < "${LISTFILE}"

echo "Fecha FIN SIZE COMPRESS: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTFILE}

