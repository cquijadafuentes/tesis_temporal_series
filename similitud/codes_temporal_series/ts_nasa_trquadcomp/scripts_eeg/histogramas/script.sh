#!/bin/bash

if [ $# -ne 1 ]; then
	echo "$(basename $0) <filelist.txt>"
	exit 1
fi

LISTFILE=$1

echo "Prcesando..."

while read p; do
	echo "${p}"
	gnuplot -e "filename='${p}'" plot
done < "${LISTFILE}"

echo "FIN"

