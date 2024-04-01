import sys
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
import pysal
import pandas as pd
from pysal.lib import weights  # Spatial weights
from pysal.explore import esda  # Exploratory Spatial analytics

if len(sys.argv) != 2:
  print("Error! en la cantidad de argumentos.")
  print(sys.argv[0], "<filename.txt>")
  sys.exit()
print("DATASET: ", sys.argv[1])

if len(sys.argv) != 2:
  print("Error! en la cantidad de argumentos.")
  print(sys.argv[0], "<filename.txt>")
  sys.exit()
print("DATASET:", sys.argv[1])

lineas = []
with open(sys.argv[1], 'r') as file:
  firstLine = True
  for line in file:
    if(firstLine):
      nums = line.split()
      rows = int(nums[0])
      cols = int(nums[1])
      print(rows, cols)
      firstLine = False
    else:
      lineas.append(line.strip())

print("Leyendo series temporales...")

numFiles = len(lineas)
print(numFiles)
tSeries = np.zeros((rows, cols, numFiles))
iFile = 0
print("Calculando matriz de pesos espaciales")
for i in lineas:
  npI = np.fromfile(i, dtype='int32')
  npI = npI.reshape((rows, cols))
  print("Calculando I de Moran")
  w = weights.Queen.from_xarray(npI)
  moran = esda.Moran(npI,w)
  print("Estadística I de Moran:", moran.I)
  sys.exit()
  #i_Moran_1T = esda.moran.Moran(matriz, w, two_tailed=False)
  #i_Moran_2T = esda.moran.Moran(matriz, w, two_tailed=True)
  #w_pysal = pysal.weights.WSP(w)
  #moran_loc = pysal.esda.moran.Moran_Local(matriz, w_pysal)
  #c_Moran = moran_loc.Is
  #print(i_Moran_1T,i_Moran_2T,c_Moran)


