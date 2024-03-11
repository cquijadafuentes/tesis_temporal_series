import sys
import math
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
from statsmodels.tsa.seasonal import seasonal_decompose

def calc_euclidean(actual, predic):
    return np.sqrt(np.sum((actual - predic) ** 2))

def dis_celdas(f0, f1, c0, c1):
  return math.sqrt((f1 - f0)**2 + (c1 - c0)**2)

if len(sys.argv) != 4:
  print("Error! en la cantidad de argumentos.")
  print(sys.argv[0], "<filename.txt> <row> <column>")
  sys.exit()
print("DATASET:", sys.argv[1])

fil = int(sys.argv[2])
col = int(sys.argv[3])

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
for i in lineas:
  npI = np.fromfile(i, dtype='int32')
  index = 0;
  for c in range(cols):
    for f in range(rows):
      tSeries[f][c][iFile] = int(npI[index])
      index += 1
  iFile += 1

for i in tSeries[fil][col]:
  print(i)

