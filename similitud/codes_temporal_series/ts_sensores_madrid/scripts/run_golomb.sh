#!/bin/bash

if [ $# -ne 2 ]; then
	echo "$(basename $0) <input_data> <groups_data>"
	exit 1
fi

DATAFILE=$1
GROUPSFILE=$2
QGOLOMB="2 4 8 16 32 64 128 256 512 1024 2048 4096 8192"

echo "Fecha INICIO: $(date +'%Y/%m/%d %H:%M:%S')"

for q in ${QGOLOMB}
do
	echo "======================================================================="
	echo "./TS_explora_vlcvector_compara_series_doble_diferencia_todos_vgolomb ${DATAFILE} ${GROUPSFILE} borrar.txt ${q}"
	./TS_explora_vlcvector_compara_series_doble_diferencia_todos_vgolomb ${DATAFILE} ${GROUPSFILE} borrar.txt ${q}
	echo "======================================================================="
done

echo "Fecha FIN: $(date +'%Y/%m/%d %H:%M:%S')"

rm borrar.txt
