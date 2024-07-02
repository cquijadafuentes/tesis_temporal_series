import sys
import numpy
import scipy.io as sio

if len(sys.argv) != 2:
  print("Error! en la cantidad de argumentos.")
  print(sys.argv[0], "<filename.mat>")
  sys.exit()

mat = sio.loadmat(sys.argv[1])
data = mat['EEG'][0]['data'][0]
electrodos = int(data.shape[0])
muestras = int(data.shape[1])
print(electrodos, muestras)

for i in range(electrodos):
  for j in range(muestras):
    print(data[i][j], end =" ")
  print()
