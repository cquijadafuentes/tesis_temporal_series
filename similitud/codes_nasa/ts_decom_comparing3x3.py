import sys
import math
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
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

print("Normalizando y descomponiendo series de tiempo...")
ts_decomposed = [[None for _ in range(cols)] for _ in range(rows)]
for f in range(rows):
  for c in range(cols):
    for ti in range(tSeries[f][c].size):
      cellValue = tSeries[f][c][ti]
      tSeries[f][c][ti] = (cellValue - ts_min)/ts_range
    ts_decomposed[f][c] = seasonal_decompose(tSeries[f][c], model='additive', period=3)

print("Valores normalizados y decomposed...")

dcts_euc_trend = np.zeros((rows, cols))
dcts_euc_seasonal = np.zeros((rows, cols))
dcts_euc_resid = np.zeros((rows, cols))

print("Calculando similitud euc entre descomposiciones")
for f in range(rows):
  for c in range(cols):
    cant = 0
    simEucTrend = 0
    simEucSeasonal = 0
    simEucResid = 0
    xTrend = np.array(ts_decomposed[f][c].trend)[2:-3]
    xSeason = np.array(ts_decomposed[f][c].seasonal)
    xResid = np.array(ts_decomposed[f][c].resid)[2:-3]
    for f1 in range(-1,2):
      for c1 in range(-1,2):
        if ((f+f1)>0 and (f+f1)<rows and (c+c1)>0 and (c+c1)<cols and (f1!=0 or c1!=0)):
          cant+=1;
          yTrend = np.array(ts_decomposed[f+f1][c+c1].trend)[2:-3]
          ySeason = np.array(ts_decomposed[f+f1][c+c1].seasonal)
          yResid = np.array(ts_decomposed[f+f1][c+c1].resid)[2:-3]
          simEucTrend += calc_euclidean(xTrend, yTrend)
          simEucSeasonal += calc_euclidean(xSeason, ySeason)
          simEucResid += calc_euclidean(xResid, yResid)
    dcts_euc_trend[f][c] = float(simEucTrend)/cant
    dcts_euc_seasonal[f][c] = float(simEucSeasonal)/cant
    dcts_euc_resid[f][c] = float(simEucResid)/cant

print("Buscando mins y maxs en similitudes euc")
minEucTrend = dcts_euc_trend[0][0]
minEucSeason = dcts_euc_seasonal[0][0]
minEucResid = dcts_euc_resid[0][0]
maxEucTrend = dcts_euc_trend[0][0]
maxEucSeason = dcts_euc_seasonal[0][0]
maxEucResid = dcts_euc_resid[0][0]
for f in range(rows):
  for c in range(cols):
    # Mínimos
    if(dcts_euc_trend[f][c] < minEucTrend):
      minEucTrend = dcts_euc_trend[f][c]
    if(dcts_euc_seasonal[f][c] < minEucSeason):
      minEucSeason = dcts_euc_seasonal[f][c]
    if(dcts_euc_resid[f][c] < minEucResid):
      minEucResid = dcts_euc_resid[f][c]
    # Máximos
    if(dcts_euc_trend[f][c] > maxEucTrend):
      maxEucTrend = dcts_euc_trend[f][c]
    if(dcts_euc_seasonal[f][c] > maxEucSeason):
      maxEucSeason = dcts_euc_seasonal[f][c]
    if(dcts_euc_resid[f][c] > maxEucResid):
      maxEucResid = dcts_euc_resid[f][c]

# Configurar paleta de colores
cmap = sns.color_palette("coolwarm", as_cmap=True)

# Crear un mapa de calor
heatmap = sns.heatmap(dcts_euc_trend, cmap=cmap, cbar_kws={'label':'Trend 3x3 Norm'}, vmin=minEucTrend, vmax=maxEucTrend)
plt.savefig(sys.argv[1] + '.heatmap.norm_decomposed_trend_3x3.png', bbox_inches='tight')
#plt.show()
plt.close()

# Crear un mapa de calor para promedios Euclideos
heatmap = sns.heatmap(dcts_euc_seasonal, cmap=cmap, cbar_kws={'label':'Seasonal 3x3 Norm'}, vmin=minEucSeason, vmax=maxEucSeason)
plt.savefig(sys.argv[1] + '.heatmap.norm_decomposed_seasonal_3x3.png', bbox_inches='tight')
#plt.show()
plt.close()

# Crear un mapa de calor para promedios por MAPE
heatmap = sns.heatmap(dcts_euc_resid, cmap=cmap, cbar_kws={'label':'Resid 3x3 Norm'}, vmin=minEucResid, vmax=maxEucResid)
plt.savefig(sys.argv[1] + '.heatmap.norm_decomposed_resid_3x3.png', bbox_inches='tight')
#plt.show()
plt.close()

