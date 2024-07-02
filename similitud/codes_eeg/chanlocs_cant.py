import sys
import numpy
import scipy.io as sio

if len(sys.argv) != 2:
  print("Error! en la cantidad de argumentos.")
  print(sys.argv[0], "<filename.mat>")
  sys.exit()

mat = sio.loadmat(sys.argv[1])
chanlocs = mat['EEG'][0]['chanlocs'][0][0]
cant = int(chanlocs.size)
print(sys.argv[1],"\tcant. chanlocs:", cant)