import sys
import math
import numpy as np

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
	print(sys.argv[0], "<filename.txt>")
	sys.exit()
	

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

numFiles = len(lineas)
print(numFiles)
rXc = rows * cols
tSeries = np.zeros((rXc, numFiles))
iFile = 0
for i in lineas:
	npI = np.fromfile(i, dtype='int32')
	index = 0;
	for c in range(rXc):
		tSeries[c][iFile] = int(npI[index])
		index += 1
	iFile += 1

fname = (str(sys.argv[1]))[:5]
analizar_correlaciones(tSeries, fname)


"""
udec_server /home/carlos/datasets/times_series/NASA
nohup python3 ~/codes_nasa/nasa_temporal_correlation.py APCP_NLDAS_FORA0125_H.txt > ~/codes_nasa/APCP.temp_correl.txt &
nohup python3 ~/codes_nasa/nasa_temporal_correlation.py CAPE_NLDAS_FORA0125_H.txt > ~/codes_nasa/CAPE.temp_correl.txt &
nohup python3 ~/codes_nasa/nasa_temporal_correlation.py CONVfrac_NLDAS_FORA0125_H.txt > ~/codes_nasa/CONV.temp_correl.txt &
nohup python3 ~/codes_nasa/nasa_temporal_correlation.py DLWRF_NLDAS_FORA0125_H.txt > ~/codes_nasa/DLWRF.temp_correl.txt &
nohup python3 ~/codes_nasa/nasa_temporal_correlation.py DSWRF_NLDAS_FORA0125_H.txt > ~/codes_nasa/DSWRF.temp_correl.txt &
nohup python3 ~/codes_nasa/nasa_temporal_correlation.py PEVAP_NLDAS_FORA0125_H.txt > ~/codes_nasa/PEVAP.temp_correl.txt &
nohup python3 ~/codes_nasa/nasa_temporal_correlation.py PRES_NLDAS_FORA0125_H.txt > ~/codes_nasa/PRES.temp_correl.txt &
nohup python3 ~/codes_nasa/nasa_temporal_correlation.py SPFH_NLDAS_FORA0125_H.txt > ~/codes_nasa/SPFH.temp_correl.txt &
nohup python3 ~/codes_nasa/nasa_temporal_correlation.py TMP_NLDAS_FORA0125_H.txt > ~/codes_nasa/TMP.temp_correl.txt &
nohup python3 ~/codes_nasa/nasa_temporal_correlation.py UGRD_NLDAS_FORA0125_H.txt > ~/codes_nasa/UGRD.temp_correl.txt &
nohup python3 ~/codes_nasa/nasa_temporal_correlation.py VGRD_NLDAS_FORA0125_H.txt > ~/codes_nasa/VGRD.temp_correl.txt &
"""

