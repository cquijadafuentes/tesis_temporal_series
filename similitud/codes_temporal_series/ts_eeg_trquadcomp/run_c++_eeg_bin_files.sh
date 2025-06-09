#!/bin/bash

if [ $# -ne 2 ]; then
	echo "$(basename $0) <EEG_filelist.txt> <executable_BIN_build>"
	exit 1
fi

LISTFILE=$1
EXECUTABLE=$2

echo "Fecha INICIO EEG BIN variante: $(date +'%Y/%m/%d %H:%M:%S')"
echo "Ejecutando ${EXECUTABLE}"

while read p; do
	./${EXECUTABLE} ${p}.bin
done < "${LISTFILE}"

echo "Fecha FIN EEG BIN variante: $(date +'%Y/%m/%d %H:%M:%S')"

