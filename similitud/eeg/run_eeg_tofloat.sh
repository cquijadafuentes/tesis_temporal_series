#!/bin/sh

if [ $# -ne 1 ]; then
  echo "$(basename $0) <filelist.txt>"
  echo "Por cada archivo en <filelist.txt> se ejecuta eeg_data_to_float.py"
  exit 1
fi

filename=${1}

while read p; do
  echo "${p}"
  python3 eeg_data_to_float ${p}.mat > ${p}_float.txt
done < "${filename}"