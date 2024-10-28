import sys
import numpy as np
import scipy.io as sio
import seaborn as sns
import matplotlib.pyplot as plt

if len(sys.argv) != 2:
	print("Error! en la cantidad de argumentos.")
	print(sys.argv[0], "<filename.mat>")
	sys.exit()

# Etiquetas por FILAS y COLUMNAS
labelsXrow = ["HEOG","VEOG","FP1","FPZ","FP2","AF3","AF4","F7","F5","F3","F1","FZ","F2","F4","F6","F8","FT7","FC5","FC3","FC1","FCZ","FC2","FC4","FC6","FT8","T7","C5","C3","C1","CZ","C2","C4","C6","T8","M1","M2","TP7","CP5","CP3","CP1","CPZ","CP2","CP4","CP6","TP8","P7","P5","P3","P1","PZ","P2","P4","P6","P8","PO7","PO5","PO3","POZ","PO4","PO6","PO8","CB1","O1","OZ","O2","CB2"]
labelsXcol = ["HEOG","VEOG","M1","F7","FT7","T7","TP7","P7","PO7","F5","FC5","C5","CP5","P5","PO5","AF3","F3","FC3","C3","CP3","P3","PO3","FP1","F1","FC1","C1","CP1","P1","O1","CB1","FPZ","FZ","FCZ","CZ","CPZ","PZ","POZ","OZ","FP2","F2","FC2","C2","CP2","P2","O2","CB2","AF4","F4","FC4","C4","CP4","P4","PO4","F6","FC6","C6","CP6","P6","PO6","F8","FT8","T8","TP8","M2","P8","PO8"]

# Diccionario con las POSICIONES EN MATRIZ de cada etiqueta
posiciones = {}
posiciones["FP1"] = [0,3]
posiciones["FPZ"] = [0,4]
posiciones["FP2"] = [0,5]
posiciones["AF3"] = [1,2]
posiciones["AF4"] = [1,6]
posiciones["F7"] = [2,0]
posiciones["F5"] = [2,1]
posiciones["F3"] = [2,2]
posiciones["F1"] = [2,3]
posiciones["FZ"] = [2,4]
posiciones["F2"] = [2,5]
posiciones["F4"] = [2,6]
posiciones["F6"] = [2,7]
posiciones["F8"] = [2,8]
posiciones["FT7"] = [3,0]
posiciones["FC5"] = [3,1]
posiciones["FC3"] = [3,2]
posiciones["FC1"] = [3,3]
posiciones["FCZ"] = [3,4]
posiciones["FC2"] = [3,5]
posiciones["FC4"] = [3,6]
posiciones["FC6"] = [3,7]
posiciones["FT8"] = [3,8]
posiciones["T7"] = [4,0]
posiciones["C5"] = [4,1]
posiciones["C3"] = [4,2]
posiciones["C1"] = [4,3]
posiciones["CZ"] = [4,4]
posiciones["C2"] = [4,5]
posiciones["C4"] = [4,6]
posiciones["C6"] = [4,7]
posiciones["T8"] = [4,8]
posiciones["M1"] = [8,0]
posiciones["TP7"] = [5,0]
posiciones["CP5"] = [5,1]
posiciones["CP3"] = [5,2]
posiciones["CP1"] = [5,3]
posiciones["CPZ"] = [5,4]
posiciones["CP2"] = [5,5]
posiciones["CP4"] = [5,6]
posiciones["CP6"] = [5,7]
posiciones["TP8"] = [5,8]
posiciones["M2"] = [8,8]
posiciones["P7"] = [6,0]
posiciones["P5"] = [6,1]
posiciones["P3"] = [6,2]
posiciones["P1"] = [6,3]
posiciones["PZ"] = [6,4]
posiciones["P2"] = [6,5]
posiciones["P4"] = [6,6]
posiciones["P6"] = [6,7]
posiciones["P8"] = [6,8]
posiciones["PO7"] = [7,0]
posiciones["PO5"] = [7,1]
posiciones["PO3"] = [7,2]
posiciones["POZ"] = [7,4]
posiciones["PO4"] = [7,6]
posiciones["PO6"] = [7,7]
posiciones["PO8"] = [7,8]
posiciones["CB1"] = [8,2]
posiciones["O1"] = [8,3]
posiciones["OZ"] = [8,4]
posiciones["O2"] = [8,5]
posiciones["CB2"] = [8,6]
posiciones["HEOG"] = [0,1]
posiciones["VEOG"] = [0,7]

# mat = sio.loadmat("/home/carlos/testing_data/627_Depression_REST.mat")

mat = sio.loadmat(sys.argv[1])
mx = max(mat['EEG'][0]['data'][0][0])
mn = min(mat['EEG'][0]['data'][0][0])
l = len(mat['EEG'][0]['chanlocs'][0][0])
ldata = len(mat['EEG'][0]['data'][0][0])

data = {}
sumas = {}

for i in range(l):
	etiqueta = str(mat['EEG'][0]['chanlocs'][0][0][i][0][0])
	if(etiqueta != "EKG"):
		d = mat['EEG'][0]['data'][0][i]
		sumas[etiqueta] = sum(d)
		mx = max(mx, max(d))
		mn = min(mn, min(d))
		data[etiqueta] = d
print("máximo:",mx," - mínimo:", mn)

print("Graficando previews del EEG")

fig, ax = plt.subplots(9, 9, figsize=(30, 20))
t = str("EEG "+ sys.argv[1])
fig.suptitle(t, y=0.99)
plt.subplots_adjust(top=0.92)
for lxr in labelsXrow:
	# Generando gráfica de previsualización
	px = posiciones[lxr][0]
	py = posiciones[lxr][1]
	ax[px][py].plot(data[lxr], color='blue')
	ax[px][py].set_title(lxr)
	#ax[px][py].title()
	ax[px][py].grid(True)
	ax[px][py].set_ylim([mn, mx])

plt.tight_layout()
plt.savefig(str(sys.argv[1]+".preview.png"))
plt.close()

print("Calculando suma de diferencias")

sum_diffs = {}
diff_proms = {}
acum_sum_diffs = {}

for e1 in labelsXrow:
	for e2 in labelsXrow:
		sd = sumas[e1] - sumas[e2]
		dp = (sumas[e1] / ldata) - (sumas[e2] / ldata)
		sum_diffs[(e1,e2)] = sd
		diff_proms[(e1,e2)] = dp
		if e1 not in acum_sum_diffs:
			acum_sum_diffs[e1] = 0
		acum_sum_diffs[e1] = acum_sum_diffs[e1] + sd

l = len(labelsXrow) # == len(labelsXcol)
matriz = np.zeros((l, l))

cmap = sns.color_palette("coolwarm", as_cmap=True)


print("Creando un mapa de calor de la suma de diferencias x fila")
for i in range(l):
	for j in range(l):
		li = labelsXrow[i]
		lj = labelsXrow[j]
		matriz[i][j] = sum_diffs[(li,lj)]

plt.figure(figsize=(14, 12))
menor = np.min(matriz)
mayor = np.max(matriz)
heatmapSF = sns.heatmap(matriz, cmap=cmap, vmin=menor, vmax=mayor, xticklabels=labelsXrow, yticklabels=labelsXrow)
plt.title('Suma de Diferencias')
plt.savefig(sys.argv[1] + '.heatmap.suma_diferencias_xRow.png', bbox_inches='tight')
plt.close()

print("Crear un mapa de calor de la diferencia de promedios x fila")
for i in range(l):
	for j in range(l):
		li = labelsXrow[i]
		lj = labelsXrow[j]
		matriz[i][j] = diff_proms[(li,lj)]
plt.figure(figsize=(14, 12))
menor = np.min(matriz)
mayor = np.max(matriz)
heatmapDP = sns.heatmap(matriz, cmap=cmap, vmin=menor, vmax=mayor, xticklabels=labelsXrow, yticklabels=labelsXrow)
plt.title('Diferencia de Promedios')
plt.savefig(sys.argv[1] + '.heatmap.diferencia_proms_xRow.png', bbox_inches='tight')
plt.close()


print("Creando un mapa de calor de la suma de diferencias x columna")
for i in range(l):
	for j in range(l):
		li = labelsXcol[i]
		lj = labelsXcol[j]
		matriz[i][j] = sum_diffs[(li,lj)]
plt.figure(figsize=(14, 12))
menor = np.min(matriz)
mayor = np.max(matriz)
heatmapSF = sns.heatmap(matriz, cmap=cmap, vmin=menor, vmax=mayor, xticklabels=labelsXcol, yticklabels=labelsXcol)
plt.title('Suma de Diferencias')
plt.savefig(sys.argv[1] + '.heatmap.suma_diferencias_xCol.png', bbox_inches='tight')
plt.close()

print("Crear un mapa de calor de la diferencia de promedios x columna")
for i in range(l):
	for j in range(l):
		li = labelsXcol[i]
		lj = labelsXcol[j]
		matriz[i][j] = diff_proms[(li,lj)]
plt.figure(figsize=(14, 12))
menor = np.min(matriz)
mayor = np.max(matriz)
heatmapDP = sns.heatmap(matriz, cmap=cmap, vmin=menor, vmax=mayor, xticklabels=labelsXcol, yticklabels=labelsXcol)
plt.title('Diferencia de Promedios')
plt.savefig(sys.argv[1] + '.heatmap.diferencia_proms_xCol.png', bbox_inches='tight')
plt.close()



##########################################


print("Creando un mapa de calor de la suma de diferencias x menor diferencia acumulada")
llavs = list(acum_sum_diffs.keys())
vals = list(acum_sum_diffs.values())

for i in range(len(llavs)):
	if(vals[i] != acum_sum_diffs[llavs[i]]):
		print("Se encontró diferencia en valores")

sorted_vals, sorted_llavs = zip(*sorted(zip(vals, llavs)))
for i in range(l):
	for j in range(l):
		li = sorted_llavs[i]
		lj = sorted_llavs[j]
		matriz[i][j] = sum_diffs[(li,lj)]
plt.figure(figsize=(14, 12))
menor = np.min(matriz)
mayor = np.max(matriz)
heatmapDP = sns.heatmap(matriz, cmap=cmap, vmin=menor, vmax=mayor, xticklabels=sorted_llavs, yticklabels=sorted_llavs)
plt.title('Diferencia de Promedios')
plt.savefig(sys.argv[1] + '.heatmap.suma_diferencias_xsumdiffacum.png', bbox_inches='tight')
plt.close()

print("Archivo:",sys.argv[1],sorted_llavs[0]," sensor con menor suma acumulada de diferencias con valor:", sorted_vals[0])
mll = llavs[0]
mv = abs(vals[0])
for i in range(len(sorted_vals)):
	if abs(sorted_vals[i]) < mv:
		mv = abs(sorted_vals[i])
		mll = sorted_llavs[i]
	print(sorted_llavs[i], sorted_vals[i], abs(sorted_vals[i]))
print("Archivo:", sys.argv[1], mll, " sensor con valor más cercano a 0 con valor:", mv)

#plt.show()
print("FIN")