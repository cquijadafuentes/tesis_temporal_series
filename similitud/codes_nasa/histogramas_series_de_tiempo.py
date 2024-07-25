import sys
import math
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt

def calc_euclidean(actual, predic):
		return np.sqrt(np.sum((actual - predic) ** 2))

def dis_celdas(f0, f1, c0, c1):
	return math.sqrt((f1 - f0)**2 + (c1 - c0)**2)

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
for i in lineas:
	npI = np.fromfile(i, dtype='int32')
	index = 0;
	for c in range(cols):
		for f in range(rows):
			tSeries[f][c][iFile] = int(npI[index])
			index += 1
	iFile += 1

print("Calculando histograma de serie promedio de todo le mapa...")
sPromedio = []
celdas = rows*cols
for t in range(numFiles):
	promedio = 0
	for f in range(rows):
		for c in range(cols):
			promedio += tSeries[f][c][t]
	promedio = int(promedio / celdas)
	sPromedio.append(promedio)
vPmin = min(sPromedio)
vPmax = max(sPromedio)

sDiferencia = []
for t in range(1, numFiles):
	d = sPromedio[t] - sPromedio[t-1]
	sDiferencia.append(d)
vDmin = min(sDiferencia)
vDmax = max(sDiferencia)

# Preparando entorno para gráficas:
fig, ax = plt.subplots()
plt.plot(sPromedio, label="Serie Promedio")
plt.plot(sDiferencia, label="Serie Diferencia del Promedio")
ax.set_yticks((vDmin, vDmax, vPmin, vPmax))
plt.title("Histograma de valores basado en Promedio")
ax.set_xlabel("t")
ax.set_ylabel("Valor")
ax.legend(loc='upper left')
ax.grid(True)
fig.set_size_inches(12, 4)
plt.tight_layout()
plt.savefig(sys.argv[1] + '.histograma_promedio.png', bbox_inches='tight')
plt.close()