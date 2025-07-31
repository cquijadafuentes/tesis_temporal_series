import sys
import struct
import numpy
import scipy.io as sio

if len(sys.argv) != 4:
	print("Error! en la cantidad de argumentos.")
	print(sys.argv[0], "<input_filename.mat> <eeg_electrode_ids.txt> <output_filename>")
	sys.exit()

mat = sio.loadmat(sys.argv[1])
data = mat['EEG'][0]['data'][0]
electrodos = int(data.shape[0])
muestras = int(data.shape[1])
print(electrodos, muestras)

electrodePosition = {}

contador_electrodos = 0
for i in range(electrodos):
	etiqueta = str(mat['EEG'][0]['chanlocs'][0][0][i][0][0])
	if(etiqueta == "EKG"):
		contador_electrodos -= 1
	electrodePosition[etiqueta] = contador_electrodos
	contador_electrodos += 1
print(contador_electrodos)



electrodesIdsFilename = str(sys.argv[2])
with open(electrodesIdsFilename, 'r') as idsfile:
	outfilename = str(sys.argv[3])
	print("Generando archivo de salida:", outfilename)
	with open(outfilename, "wb") as outfile:
		outfile.write(struct.pack('i', contador_electrodos))
		outfile.write(struct.pack('i', muestras))
		for linea in idsfile:
			sl = linea.strip()
			if sl in electrodePosition:
				pos = electrodePosition[sl]
				for j in range(muestras):
					num = int(data[pos][j] * 100)
					outfile.write(struct.pack('i', num))

print("FIN")


