#!/bin/sh

if [ $# -ne 2 ]; then
  echo "$(basename $0) <filelist.txt> <python_code.py>"
  echo "Por cada archivo en <filelist.txt> se ejecuta <python_code.py>."
  exit 1
fi

filename=${1}
runfile=${2}

while read p; do
  echo "${p}"
  python3 ${runfile} ${p}.mat
done < "${filename}"