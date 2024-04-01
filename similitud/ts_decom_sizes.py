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

print("Leyendo y descomponiendo series temporales...")

ts_max = sys.float_info.min
ts_min = sys.float_info.max

numFiles = len(lineas)
print("numFiles:",numFiles)
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

ts_range = float(ts_max - ts_min)
print("min:", ts_min, "max:", ts_max, "rango:", ts_range)

dt_max = sys.float_info.min
dt_min = sys.float_info.max
ds_max = sys.float_info.min
ds_min = sys.float_info.max
dr_max = sys.float_info.min
dr_min = sys.float_info.max

print("Descomponiendo series de tiempo...")
ts_decomposed = [[None for _ in range(cols)] for _ in range(rows)]
for f in range(rows):
  for c in range(cols):
    ts_decomposed[f][c] = seasonal_decompose(tSeries[f][c], model='additive', period=3)
    xTrend = np.array(ts_decomposed[f][c].trend)[2:-3]
    mi = min(xTrend)
    if mi < dt_min:
      dt_min = mi
    ma = max(xTrend)
    if ma > dt_max:
      dt_max = ma
    xSeason = np.array(ts_decomposed[f][c].seasonal)
    mi = min(xSeason)
    if mi < ds_min:
      ds_min = mi
    ma = max(xSeason)
    if ma > ds_max:
      ds_max = ma
    xResid = np.array(ts_decomposed[f][c].resid)[2:-3]
    mi = min(xResid)
    if mi < dr_min:
      dr_min = mi
    ma = max(xResid)
    if ma > dr_max:
      dr_max = ma

ts_range = ts_max - ts_min
dt_range = dt_max - dt_min
ds_range = ds_max - ds_min
dr_range = dr_max - dr_min

print("Rango Serie Temporal:", ts_min, "-", ts_max, "rango:", ts_range)
print("Rango Trend:", dt_min, "-", dt_max, "rango:", dt_range)
print("Rango Seasonal:", ds_min, "-", ds_max, "rango:", ds_range)
print("Rango Resid:", dr_min, "-", dr_max, "rango:", dr_range)

totalItems = rows * cols

minbitsTS = math.ceil(math.log2(ts_range))
totalBitsTS = minbitsTS*totalItems
print("Bits para Serie Temporal:", totalBitsTS)
minbits = math.ceil(math.log2(dt_range))
totalBits = minbits*totalItems
print("Bits para Trend:", totalBits,"- (", 100*totalBits/totalBitsTS, "%)")
minbits = math.ceil(math.log2(ds_range))
totalBits = minbits*totalItems
print("Bits para Seasonal:", totalBits,"- (", 100*totalBits/totalBitsTS, "%)")
minbits = math.ceil(math.log2(dr_range))
totalBits = minbits*totalItems
print("Bits para Resid:", totalBits,"- (", 100*totalBits/totalBitsTS, "%)")

