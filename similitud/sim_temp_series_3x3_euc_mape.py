import sys
import math
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt

def calc_euclidean(actual, predic):
    return np.sqrt(np.sum((actual - predic) ** 2))

def calc_mape(actual, predic):
    return np.mean(np.abs((actual - predic) / (actual + 0.000001)))


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
print("numFiles:",numFiles)
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

simPromEuc = np.zeros((rows, cols))
simPromMape = np.zeros((rows, cols))
print("Calculando similitudes")
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

print("Buscando mins y maxs en promedios")
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
# Crear un mapa de calor para promedios Euclideos
heatmap = sns.heatmap(simPromEuc, cmap=cmap, cbar_kws={'label':'Promedio Euclideo'}, vmin=minEuc, vmax=maxEuc)
plt.savefig(sys.argv[1] + '.heatmap.prom3x3Euc.png', bbox_inches='tight')
#plt.show()
plt.close()
# Crear un mapa de calor para promedios por MAPE
heatmap = sns.heatmap(simPromMape, cmap=cmap, cbar_kws={'label':'Promedio MAPE'}, vmin=minMape, vmax=maxMape)
plt.savefig(sys.argv[1] + '.heatmap.prom3x3MAPE.png', bbox_inches='tight')
#plt.show()
plt.close()