import sys
import numpy as np
import scipy.io as sio
import seaborn as sns
import matplotlib.pyplot as plt


def analizar_correlaciones(datos, title):
	"""Realiza análisis de correlación temporal"""
	
	# Correlación temporal (autocorrelación con diferentes lags)
	lags = [1, 2, 5, 10]  # Lags temporales a analizar
	autocorrs = {f'Lag_{lag}': [] for lag in lags}
	
	for serie in datos:
		for lag in lags:
			if lag >= len(serie):
				autocorr = np.nan
			else:
				autocorr = np.corrcoef(serie[:-lag], serie[lag:])[0,1]
			autocorrs[f'Lag_{lag}'].append(autocorr)	
	# Estadísticas temporales
	print("EEG\tLag_1\tLag_2\tLag_5\tLag_10\t%>.5L_2\t%>.5L_5\t%>.5L_10")
	print(title, end="\t")
	for lag in autocorrs:
		vals = np.array(autocorrs[lag])
		print(f"{np.nanmean(vals):.3f}", end="\t")
	for lag in autocorrs:
		print(f"{np.mean(vals > 0.5)*100:.1f}", end="\t")
	print("")

if len(sys.argv) != 2:
	print("Error! en la cantidad de argumentos.")
	print(sys.argv[0], "<filename.mat>")
	sys.exit()

# mat = sio.loadmat("/home/carlos/testing_data/627_Depression_REST.mat")

mat = sio.loadmat(sys.argv[1])
mx = max(mat['EEG'][0]['data'][0][0])
mn = min(mat['EEG'][0]['data'][0][0])
l = len(mat['EEG'][0]['chanlocs'][0][0])
ldata = len(mat['EEG'][0]['data'][0][0])

fname = (str(sys.argv[1]))[:3]
data = []
for i in range(l):
	etiqueta = str(mat['EEG'][0]['chanlocs'][0][0][i][0][0])
	if(etiqueta != "EKG"):
		d = mat['EEG'][0]['data'][0][i]
		data.append(d)

analizar_correlaciones(data, fname)
