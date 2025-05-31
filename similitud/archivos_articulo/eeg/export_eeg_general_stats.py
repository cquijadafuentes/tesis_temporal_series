'''
			len			min			max			mean	sd
Promedio	248474,1	-1284629,2	1283659,1	249,4	31067,0
Minimo		116692,0	-64000000,0	504,5		-68,5	22,5
Max			290020,0	-416,6		63955080,0	15123,5	1605167,3


'''


import sys
import numpy as np
import scipy.io as sio
import seaborn as sns
import matplotlib.pyplot as plt


ids = ["FP1", "FPZ", "FP2", "AF3", "AF4", "F7", "F5", "F3", "F1", "FZ", "F2", "F4", "F6", "F8", "FT7", "FC5", "FC3", "FC1", "FCZ", "FC2", "FC4", "FC6", "FT8", "T7", "C5", "C3", "C1", "CZ", "C2", "C4", "C6", "T8", "M1", "TP7", "CP5", "CP3", "CP1", "CPZ", "CP2", "CP4", "CP6", "TP8", "M2", "P7", "P5", "P3", "P1", "PZ", "P2", "P4", "P6", "P8", "PO7", "PO5", "PO3", "POZ", "PO4", "PO6", "PO8", "CB1", "O1", "OZ", "O2", "CB2", "HEOG", "VEOG"]

if len(sys.argv) != 2:
	print("Error! en la cantidad de argumentos.")
	print(sys.argv[0], "<filename.mat>")
	print("id\tlen\tmin\tmax\tmean\tsd")

# mat = sio.loadmat("/home/carlos/testing_data/627_Depression_REST.mat")

mat = sio.loadmat(sys.argv[1])
l = len(mat['EEG'][0]['chanlocs'][0][0])
ldata = len(mat['EEG'][0]['data'][0][0])
fname = (str(sys.argv[1]))[:3]
stats = np.zeros(4)

nsensores = len(ids)
fulldata = []

for i in range(l):
	etiqueta = str(mat['EEG'][0]['chanlocs'][0][0][i][0][0])
	if(etiqueta != "EKG"):
		pos = ids.index(etiqueta)
		d = mat['EEG'][0]['data'][0][i]
		for v in d:
			fulldata.append(v)

stats[0] = np.min(fulldata)
stats[1] = np.max(fulldata)
stats[2] = np.mean(fulldata)
stats[3] = np.std(fulldata)

print(f'{fname}\t{ldata}', end="")
for x in stats:
	print(f'\t{x}', end="")
print("")

