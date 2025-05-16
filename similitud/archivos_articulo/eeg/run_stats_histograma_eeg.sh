#!/bin/sh

if [ $# -ne 2 ]; then
	echo "$(basename $0) <filelist.txt> <c++_executable>"
	echo "Por cada archivo en <filelist.txt> se ejecuta <c++_executable>."
	exit 1
fi

filename=${1}
runfile=${2}

while read p; do
	./${runfile} ${p}.bin ${p}.histograma.data
done < "${filename}"

