import sys
import math
import numpy as np
from statsmodels.tsa.seasonal import seasonal_decompose

def calc_euclidean(actual, predic):
    return np.sqrt(np.sum((actual - predic) ** 2))

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

bitsO = 0
bitsT = 0
bitsS = 0
bitsR = 0
bitsOmT = 0


print("Descomponiendo series de tiempo...")
ts_decomposed = [[None for _ in range(cols)] for _ in range(rows)]
for f in range(rows):
  for c in range(cols):
    mi = min(tSeries[f][c])
    ma = max(tSeries[f][c])
    rango = int(ma-mi)
    if(rango != 0):
      bitsO += (math.ceil(math.log2(rango)) * numFiles)

    ts_decomposed[f][c] = seasonal_decompose(tSeries[f][c], model='additive', period=3)
    xTrend = np.array(ts_decomposed[f][c].trend)[2:-3]
    mi = min(xTrend)
    ma = max(xTrend)
    rango = int(ma-mi)
    if(rango != 0):
      bitsT += (math.ceil(math.log2(rango)) * numFiles)

    xSeason = np.array(ts_decomposed[f][c].seasonal)
    mi = min(xSeason)
    ma = max(xSeason)
    rango = int(ma-mi)    
    if(rango != 0):
      bitsS += (math.ceil(math.log2(rango)) * numFiles)

    xResid = np.array(ts_decomposed[f][c].resid)[2:-3]
    mi = min(xResid)
    ma = max(xResid)
    rango = int(ma-mi)
    if(rango != 0):
      bitsR += (math.ceil(math.log2(rango)) * numFiles)

    xOminusT = tSeries[f][c][2:-3] - xTrend
    mi = min(xOminusT)
    ma = max(xOminusT)
    rango = int(ma-mi)
    if(rango != 0):
      bitsOmT += (math.ceil(math.log2(rango)) * numFiles)

print("serie", "original", "trend", "season", "resid", "org-trend")
print(sys.argv[1], bitsO, bitsT, bitsS, bitsR, bitsOmT)
