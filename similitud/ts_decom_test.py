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

fila = int(sys.argv[2])
columna = int(sys.argv[3])

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

nombre = str(fila)+"_"+str(columna)
print("Mostrando desde la celda "+nombre)

for i in range(-1,2):
  for j in range(-1,2):
    xFila = fila+i
    xColumna = columna+j
    nombre = str(xFila)+"_"+str(xColumna)

    plt.figure(figsize=(9, 3))
    plt.plot(tSeries[xFila][xColumna], label='Original Time Series '+nombre)
    plt.legend()
    plt.savefig(sys.argv[1] + '.timeserie.'+nombre+'.png', bbox_inches='tight')
    #plt.show()
    plt.close()

    # Iniciando la descomposición de la serie de tiempo
    result_add = seasonal_decompose(tSeries[xFila][xColumna], model='additive', period=24)

    plt.figure(figsize=(9, 3)) 
    plt.plot(result_add.trend, label='Additive Trend '+nombre)
    plt.legend()
    plt.savefig(sys.argv[1] + '.timeserie_trend.'+nombre+'.png', bbox_inches='tight')
    #plt.show()
    plt.close()

    plt.figure(figsize=(9, 3))
    plt.plot(result_add.seasonal, label='Additive Seasonal '+nombre)
    plt.savefig(sys.argv[1] + '.timeserie_seasonal.'+nombre+'.png', bbox_inches='tight')
    plt.legend()
    #plt.show()
    plt.close()
