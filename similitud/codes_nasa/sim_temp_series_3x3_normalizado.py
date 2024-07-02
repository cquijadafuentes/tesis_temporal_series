import sys
import math
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt

def calc_euclidean(actual, predic):
    return np.sqrt(np.sum((actual - predic) ** 2))

def calc_mape(actual, predic):
    return np.mean(np.abs((actual - predic) / (abs(actual) + 0.000001)))


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
proms = np.zeros((rows, cols))

print("Normalizando y promediando los valores de las series de tiempo...")
for f in range(rows):
  for c in range(cols):
    proms[f][c] = float(0)
    for ti in range(tSeries[f][c].size):
      cellValue = tSeries[f][c][ti]
      tSeries[f][c][ti] = (cellValue - ts_min)/ts_range
      proms[f][c] += tSeries[f][c][ti]
    proms[f][c] = proms[f][c]/numFiles

print("Valores normalizados y promediados...")

simPromEuc = np.zeros((rows, cols))
simPromMape = np.zeros((rows, cols))
print("Calculando similitudes euc y mape")
for f in range(rows):
  for c in range(cols):
    cant = 0
    simEuc = 0
    simMape = 0
    for f1 in range(-1,2):
      for c1 in range(-1,2):
        if ((f+f1)>0 and (f+f1)<rows and (c+c1)>0 and (c+c1)<cols and (f1!=0 or c1!=0)):
          cant+=1;
          simEuc += calc_euclidean(tSeries[f][c], tSeries[f+f1][c+c1])
          simMape += calc_mape(tSeries[f][c], tSeries[f+f1][c+c1])
    simPromEuc[f][c] = float(simEuc)/cant
    simPromMape[f][c] = float(simMape)/cant

print("Buscando mins y maxs en similitudes euc y mape")
minEuc = simPromEuc[0][0]
minMape = simPromMape[0][0]
maxEuc = simPromEuc[0][0]
maxMape = simPromMape[0][0]
for f in range(rows):
  for c in range(cols):
    if(simPromEuc[f][c] < minEuc):
      minEuc = simPromEuc[f][c]
    if(simPromMape[f][c] < minMape):
      minMape = simPromMape[f][c]
    if(simPromEuc[f][c] > maxEuc):
      maxEuc = simPromEuc[f][c]
    if(simPromMape[f][c] > maxMape):
      maxMape = simPromMape[f][c]

# Configurar paleta de colores
cmap = sns.color_palette("coolwarm", as_cmap=True)

# Crear un mapa de calor
heatmap = sns.heatmap(proms, cmap=cmap, cbar_kws={'label':'Promedio'}, vmin=0, vmax=1)
plt.savefig(sys.argv[1] + '.heatmap.promedio_normalizado.png', bbox_inches='tight')
#plt.show()
plt.close()

# Crear un mapa de calor para promedios Euclideos
heatmap = sns.heatmap(simPromEuc, cmap=cmap, cbar_kws={'label':'Promedio Sim Euclideo (3x3)'}, vmin=minEuc, vmax=maxEuc)
plt.savefig(sys.argv[1] + '.heatmap.prom3x3Euc_normalizado.png', bbox_inches='tight')
#plt.show()
plt.close()

# Crear un mapa de calor para promedios por MAPE
heatmap = sns.heatmap(simPromMape, cmap=cmap, cbar_kws={'label':'Promedio Sim MAPE (3x3)'}, vmin=minMape, vmax=maxMape)
plt.savefig(sys.argv[1] + '.heatmap.prom3x3MAPE_normalizado.png', bbox_inches='tight')
#plt.show()
plt.close()