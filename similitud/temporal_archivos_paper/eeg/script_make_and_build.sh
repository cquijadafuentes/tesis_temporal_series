#!/bin/bash

echo "compiling..."
make all
echo "done"

echo "building test"
DEMOFILE="./demo_data/530_Depression_REST.mat"
IDSFILE="./eeg_electrode_ids.txt"
MATRIXFILE="./TRQC_eeg_build_from_bin_matrix_positions.txt"
mkdir "temp_data"
BIN_FILE="./temp_data/530.bin"
python ./eeg_data_to_bin.py ${DEMOFILE} ${IDSFILE} ${BIN_FILE}
TRQC_FILE="./temp_data/530.trqc"
./TRQC_eeg_build_from_bin ${BIN_FILE} ${MATRIXFILE} 3 ${TRQC_FILE}
echo "done"

echo "end"

exit 0
