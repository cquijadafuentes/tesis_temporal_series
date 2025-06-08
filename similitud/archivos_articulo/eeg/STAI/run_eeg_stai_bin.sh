#!/bin/sh

if [ $# -ne 4 ]; then
  echo "$(basename $0) <filelist.txt> <c++_code> <matrix_w.txt> <outputfile>"
  echo "Por cada archivo en <filelist.txt> se ejecuta <c++_code> usando la matriz"
  echo "de pesos <matrix_w.txt> y los resultados se guardan en <outputfile>"
  exit 1
fi

list=${1}
runfile=${2}
matrix=${3}
outfile=${4}


echo "Iniciado en: $(date +'%Y/%m/%d %H:%M:%S')" > ${outfile}
echo "Runing: ${0} ${1} ${2} ${3} ${4}"
while read p; do
  echo "Procesando ${p}"
  ./${runfile} ${p}.bin ${matrix} >> ${outfile}
done < "${list}"

echo "Finalizado en: $(date +'%Y/%m/%d %H:%M:%S')" >> ${outfile}

