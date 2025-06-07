import sys
import numpy
import scipy.io as sio

if len(sys.argv) < 2:
	print("Error! en la cantidad de argumentos.")
	print(sys.argv[0], "<filename.mat> [<n_elements>]")
	sys.exit()

mat = sio.loadmat(sys.argv[1])
data = mat['EEG'][0]['data'][0]
electrodos = int(data.shape[0])
muestras = int(data.shape[1])
print(electrodos, muestras)
n = muestras
if len(sys.argv) > 2:
	n = int(sys.argv[2])
for j in range(n):
	print(data[0][j], end =" ")
print()
