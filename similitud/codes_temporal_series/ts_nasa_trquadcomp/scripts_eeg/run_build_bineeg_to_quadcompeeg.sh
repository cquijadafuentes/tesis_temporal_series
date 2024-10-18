#!/bin/bash

if [ $# -ne 5 ]; then
	echo "$(basename $0) <EEG_filelist.txt> <executable_QuadComp_build> <mapping_list> <cuad_size> <output_file>"
	exit 1
fi

LISTFILE=$1
EXECBUILD=$2
MAPLIST=$3
QUADSIZE=$4
OUTFILE=$5

echo "Fecha INICIO QUADCOMP BUILD: $(date +'%Y/%m/%d %H:%M:%S')" > ${OUTFILE}

while read p; do
  echo "Procesando ${p}" >> ${OUTFILE}
  ./${EXECBUILD} ${p}.eeg.bin ${MAPLIST} ${QUADSIZE} ${p}.qc${QUADSIZE} >> ${OUTFILE}

done < "${LISTFILE}"

echo "Fecha FIN QUADCOMP BUILD: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTFILE}

