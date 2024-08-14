import sys
import math
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt

def calc_euclidean(actual, predic):
		return np.sqrt(np.sum((actual - predic) ** 2))

def dis_celdas(f0, f1, c0, c1):
	return math.sqrt((f1 - f0)**2 + (c1 - c0)**2)

def esFija(x):
	for i in range(1,len(x)):
		if x[i] != x[i-1]:
			return False
	return True

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

print("Calculando series promedio...")
promSinFijas = np.zeros(numFiles)
promConFijas = np.zeros(numFiles)
limSinFijas = []
limConFijas = []
seriesNoFijas = 0
celdas = rows*cols
fijas = []
for f in range(rows):
	for c in range(cols):
		if not esFija(tSeries[f][c]):
			promSinFijas = promSinFijas + tSeries[f][c]
			limSinFijas.append(min(tSeries[f][c]))
			limSinFijas.append(max(tSeries[f][c]))
			seriesNoFijas += 1
		else:
			fijas.append(f*cols + c)
		promConFijas += tSeries[f][c]
		limConFijas.append(min(tSeries[f][c]))
		limConFijas.append(max(tSeries[f][c]))
print(promConFijas)
print(promSinFijas)
print(celdas, seriesNoFijas)

rangoMinCF = int(min(limConFijas))
rangoMaxCF = int(max(limConFijas))
rangoMinSF = int(min(limSinFijas))
rangoMaxSF = int(max(limSinFijas))

for k in range(numFiles):
	promConFijas[k] = int(promConFijas[k] / celdas)
	promSinFijas[k] = int(promSinFijas[k] / seriesNoFijas)
print(promConFijas)
print(promSinFijas)
minPCF = int(min(promConFijas))
maxPCF = int(max(promConFijas))
minPSF = int(min(promSinFijas))
maxPSF = int(max(promSinFijas))

diffCF = []
diffSF = []
for t in range(1, numFiles):
	d = promConFijas[t] - promConFijas[t-1]
	diffCF.append(d)
	d = promSinFijas[t] - promSinFijas[t-1]
	diffSF.append(d)
minDCF = int(min(diffCF))
maxDCF = int(max(diffCF))
minDSF = int(min(diffSF))
maxDSF = int(max(diffSF))


# Preparando entorno para gráficas:
fig, axs = plt.subplots(2, figsize=(10, 8))
fig.suptitle("Serie Promedio de " + sys.argv[1][:5], fontsize=16)
# sub-figura con series fijas
axs[0].plot(promConFijas, label="Promedio")
axs[0].plot(diffCF, label="Diferencia")
axs[0].set_yticks((minDCF, maxDCF, minPCF, maxPCF))
axs[0].set_title("Promedio CON series fijas")
axs[0].set_xlabel("Instantes")
axs[0].set_ylabel("Valor")
axs[0].grid(True)
# sub-figura sin series fijas
axs[1].plot(promSinFijas, label="Promedio")
axs[1].plot(diffSF, label="Diferencia")
axs[1].set_yticks((minDSF, maxDSF, minPSF, maxPSF))
axs[1].set_title("Promedio SIN series fijas")
axs[1].set_xlabel("Instantes")
axs[1].set_ylabel("Valor")
axs[1].grid(True)
# Generar una leyenda general para la figura
handles, labels = axs[0].get_legend_handles_labels()
fig.legend(handles, labels, loc='upper right')
# Ajustar el layout para que no se solapen las subfiguras
plt.tight_layout()
nombreArchivo = sys.argv[1] + ".serie_promedio_v2.png"
plt.savefig(nombreArchivo, bbox_inches='tight')
#plt.show()
plt.close()

print("\tCalculando frecuencia de valores")
histo = {}
for f in range(rows):
	for c in range(cols):
		if (f*cols + c) not in fijas:
			for v in range(numFiles):
				p = tSeries[f][c][v]
				if p in histo:
					histo[p] += 1
				else:
					histo[p] = 1

valor = []
cantidad = []
for x in range(rangoMinSF, rangoMaxSF+1):
	valor.append(x)
	if x in histo:
		cantidad.append(histo[x])
	else:
		cantidad.append(0)
fig, ax = plt.subplots()
plt.plot(valor, cantidad)
plt.title("Histograma de valores en " + sys.argv[1][:5])
ax.set_xlabel("Valor")
ax.set_ylabel("Cantidad")
fig.set_size_inches(12, 4)
texto = "SIN series Fijas: " + str(seriesNoFijas)
texto += "\n[" + str(rangoMinSF) + "," + str(rangoMaxSF) + "]\n"
texto += "Series Fijas: " + str(len(fijas))
texto += "\n[" + str(rangoMinCF) + "," + str(rangoMaxCF) + "]"
plt.text(0.8, 0.8, texto, transform = ax.transAxes)
#plt.show()
plt.savefig(sys.argv[1] + '.histograma_serie_v2.png', bbox_inches='tight')
plt.close()
