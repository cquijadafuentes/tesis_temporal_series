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

print("Calculando serie promedio de todo el mapa...")
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
fig, ax = plt.subplots(2, figsize=(12, 8))
ax[0].plot(sPromedio, label="Serie Promedio")
ax[0].plot(sDiferencia, label="Diferencias de la Serie Promedio")
ax[0].set_yticks((vDmin, vDmax, vPmin, vPmax))
ax[0].set_xlabel("t")
ax[0].set_ylabel("Valor")
ax[0].set_title("Serie Promedio " + sys.argv[1][:5])
ax[0].legend(loc='upper left')
ax[0].grid(True)


print("Calculando histograma de todo el mapa")
print("\tCalculando minimo y máximo del mapa")
minmin = min(tSeries[0][0])
maxmax = max(tSeries[0][0])
fijas = []
for f in range(rows):
	for c in range(cols):
		smin = min(tSeries[f][c])
		smax = max(tSeries[f][c])
		if smin < minmin:
			minmin = smin
		if smax > maxmax:
			maxmax = smax
		if smin == smax:
			# Es una serie fija
			fijas.append(f*rows + c)

minmin = int(minmin)
maxmax = int(maxmax)

print("\tCalculando frecuencia de valores")
histo = {}
for f in range(rows):
	for c in range(cols):
		if (f*rows + c) not in fijas:
			for v in range(numFiles):
				p = tSeries[f][c][v] - minmin
				if p in histo:
					histo[p] += 1
				else:
					histo[p] = 1

valor = []
cantidad = []
for x in range(minmin, maxmax+1):
	valor.append(x)
	vh = int(x-minmin)
	if vh in histo:
		cantidad.append(histo[vh])
	else:
		cantidad.append(0)
cantB = maxmax - minmin + 1
ax[1].plot(valor, cantidad)
ax[1].set_xlabel("t")
ax[1].set_ylabel("Valor")
ax[1].set_title("Histograma " + sys.argv[1][:5])
texto = "Valor mínimo: " + str(minmin) + "\nValor máximo: " + str(maxmax) + "\nSeries fijas: " + str(len(fijas))
plt.text(0.8, 0.8, texto)
#plt.show()
plt.savefig(sys.argv[1] + '.serie_promedio_histograma.png', bbox_inches='tight')
plt.close()