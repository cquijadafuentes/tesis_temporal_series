import sys
import math
import numpy as np


if len(sys.argv) != 2:
  print("Error! en la cantidad de argumentos.")
  print(sys.argv[0], "<filename.txt>")
  sys.exit()

lineas = []
with open(sys.argv[1], 'r') as file:
  firstLine = True
  for line in file:
    if(firstLine):
      nums = line.split()
      rows = int(nums[0])
      cols = int(nums[1])
      firstLine = False
    else:
      lineas.append(line.strip())

ts_max = sys.float_info.min
ts_min = sys.float_info.max

numFiles = len(lineas)
tSeries = np.zeros((rows, cols, numFiles))
iFile = 0
for i in lineas:
  npI = np.fromfile(i, dtype='int32')
  index = 0;
  for c in range(cols):
    for f in range(rows):
      tSeries[f][c][iFile] = npI[index]
      if npI[index] > ts_max:
        ts_max = npI[index]
      if npI[index] < ts_min:
        ts_min = npI[index]
      index += 1
  iFile += 1

for x in range(cols):
  print(int(tSeries[0][x][0]))