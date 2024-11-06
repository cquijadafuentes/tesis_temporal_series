#!/bin/bash

if [ $# -ne 5 ]; then
	echo "$(basename $0) <EEG_filelist.txt> ./<executable_BIN_build> ./<executable_QuadComp_build> <mapping_list> <cuad_size> <output_file>"
	exit 1
fi

LISTFILE=$1
BUILDBIN=$2
BUILDNQC=$3
MAPLIST=$4
QUADSIZE=$5
OUTFILE=$6

echo "Fecha INICIO EEG BIN and QUADCOMP construction: $(date +'%Y/%m/%d %H:%M:%S')" > ${OUTFILE}

while read p; do
	echo "Procesando ${p}" >> ${OUTFILE}
	${BUILDBIN} ${p}_float.txt ${p}.bin >> ${OUTFILE}
	echo "BIN construido." >> ${OUTFILE}
	${EXECBUILD} ${p}.eeg.bin ${MAPLIST} ${QUADSIZE} ${p}.qc${QUADSIZE} >> ${OUTFILE}
	echo "NQC construido." >> ${OUTFILE}

done < "${LISTFILE}"

echo "Fecha FIN QUADCOMP BUILD: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTFILE}

