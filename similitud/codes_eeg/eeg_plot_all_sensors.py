import sys
import numpy as np
import scipy.io as sio
import matplotlib.pyplot as plt

# mat = sio.loadmat("/home/carlos/testing_data/627_Depression_REST.mat")

if len(sys.argv) != 2:
	print("Error! en la cantidad de argumentos.")
	print(sys.argv[0], "<filename.mat>")
	sys.exit()

mat = sio.loadmat(sys.argv[1])
data = mat['EEG'][0]['data'][0]
labels = []
mx = max(data[0])
mn = min(data[0])
l = len(mat['EEG'][0]['chanlocs'][0][0])

for i in range(l):
	labels.append(str(mat['EEG'][0]['chanlocs'][0][0][i][0][0]))
	mx = max(mx, max(data[i]))
	mn = min(mn, min(data[i]))
print("máximo:",mx," - mínimo:", mn)

# Diccionario con las posiciones de cada etiqueta
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

fig, ax = plt.subplots(9, 9, figsize=(30, 20))
for i in range(l):
	if labels[i] in posiciones:
		px = posiciones[labels[i]][0]
		py = posiciones[labels[i]][1]
		ax[px][py].plot(data[i], color='blue')
		#ax[px][py].title()
		ax[px][py].grid(True)
		ax[px][py].set_ylim([mn, mx])
	else:
		print("Lablel",labels[i],"no esta en archivo",sys.argv[1])

plt.tight_layout()
plt.savefig(str(sys.argv[1]+".preview.png"))