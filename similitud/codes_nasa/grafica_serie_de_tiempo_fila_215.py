import sys
import math
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt

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

# Se revisan en la fila 215 las columnas 10,80,150,220
ftp = 215
ctp = [10,80,150,220]
# Preparando entorno para gráficas:
fig, ax = plt.subplots()

for c in ctp:
	lab = "Serie [" + str(ftp) + "][" + str(c) + "]"
	print(tSeries[ftp][c])
	plt.plot(tSeries[ftp][c], label=lab)

plt.title("Series de tiempo a intervalos de 70 celdas de distancia en fila "+str(ftp))
ax.set_xlabel("t")
ax.set_ylabel("Valor")
ax.legend(loc='upper left')
ax.grid(True)
fig.set_size_inches(12, 4)
plt.tight_layout()
nombreArchivo = sys.argv[1] + ".series_lejanas_f" + str(ftp) + ".png"
plt.savefig(nombreArchivo, bbox_inches='tight')
plt.close()

deltas = [[0,0],[0,1],[-1,0],[0,-1],[1,0]]

fig, axs = plt.subplots(len(deltas), figsize=(10, 8))
fig.suptitle("Comparación de series de tiempo fila " + str(ftp), fontsize=16)
for c in ctp:
	for i in range(len(deltas)):
		pf = ftp + deltas[i][0]
		pc = c + deltas[i][1]
		print("Mostrando celda",pf,pc)
		lab = "[" + str(pf) + "][" + str(pc) + "]"
		print(tSeries[pf][pc])
		axs[i].plot(tSeries[pf][pc], label=lab)
		tit = "[" + str(pf) + "][" + str(pc) + "]"
		axs[i].set_title(tit)
		axs[i].set_xlabel("t")
		axs[i].set_ylabel("Valor")
# Generar una leyenda general para la figura
handles, labels = axs[0].get_legend_handles_labels()
fig.legend(handles, labels, loc='upper right')
# Ajustar el layout para que no se solapen las subfiguras
plt.tight_layout()
nombreArchivo = sys.argv[1] + ".series_cercanas_f" + str(ftp) + ".png"
plt.savefig(nombreArchivo, bbox_inches='tight')
plt.close()

