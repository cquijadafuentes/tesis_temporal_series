import sys
import math
import numpy as np
import matplotlib.pyplot as plt

def distance(f0, f1, c0, c1):
  return math.sqrt((f1 - f0)**2 + (c1 - c0)**2)

if len(sys.argv) != 2:
  print("Error! en la cantidad de argumentos.")
  print(sys.argv[0], "<filename.txt>")
  sys.exit()
print("File Dataset:", sys.argv[1])

lineas = []
with open(sys.argv[1], 'r') as file:
  firstLine = True
  for line in file:
    if(firstLine):
      nums = line.split()
      rows = int(nums[0])
      cols = int(nums[1])
      print("Dimensiones grilla:", rows, cols)
      firstLine = False
    else:
      lineas.append(line.strip())

print("Leyendo series temporales...")

ts_max = sys.float_info.min
ts_min = sys.float_info.max

numFiles = len(lineas)
print("numFiles:",numFiles)
iFile = 0
N = rows*cols
maxDist = distance(0, rows, 0, cols)
print(maxDist)
print("Calculando matriz de pesos...")
W = np.zeros((N,N))
i = 0;
i1 = 0;
for f in range(rows):
  for c in range(cols):
    for f1 in range(rows):
      for c1 in range(cols):
        if(f != f1 and c != c1):
          d = distance(f, f1, c, c1)
          W[i][i1] = 1 - (d / maxDist)
        i1+=1
    i+=1
print(W)
sys.exit()
for i in lineas:
  X = np.fromfile(i, dtype='int32')
  index = 0;
  meanX = X.mean()
  #for i in range(rows):
  #  for j in range(cols):

  
