import sys
import math
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt

"""

nohup python3 final_caracteristicas_datos_nasa.py ~/datasets/times_series/NASA/ APCP.stats.txt ./ &
nohup python3 final_caracteristicas_datos_nasa.py ~/datasets/times_series/NASA/ CAPE.stats.txt ./ &
nohup python3 final_caracteristicas_datos_nasa.py ~/datasets/times_series/NASA/ CONVfrac.stats.txt ./ &
nohup python3 final_caracteristicas_datos_nasa.py ~/datasets/times_series/NASA/ DLWRF.stats.txt ./ &
nohup python3 final_caracteristicas_datos_nasa.py ~/datasets/times_series/NASA/ DSWRF.stats.txt ./ &
nohup python3 final_caracteristicas_datos_nasa.py ~/datasets/times_series/NASA/ PEVAP.stats.txt ./ &
nohup python3 final_caracteristicas_datos_nasa.py ~/datasets/times_series/NASA/ PRES.stats.txt ./ &
nohup python3 final_caracteristicas_datos_nasa.py ~/datasets/times_series/NASA/ SPFH.stats.txt ./ &
nohup python3 final_caracteristicas_datos_nasa.py ~/datasets/times_series/NASA/ TMP.stats.txt ./ &
nohup python3 final_caracteristicas_datos_nasa.py ~/datasets/times_series/NASA/ UGRD.stats.txt ./ &
nohup python3 final_caracteristicas_datos_nasa.py ~/datasets/times_series/NASA/ VGRD.stats.txt ./ &
"""


if len(sys.argv) != 4:
	print("Error! en la cantidad de argumentos.")
	print(sys.argv[0], "<input_folder/> <filename.txt> <output_folder>")
	sys.exit()
inputFolder = str(sys.argv[1])
filename = str(sys.argv[2])
outputFolder = str(sys.argv[3])
fullFileName = inputFolder + filename

print("Folder: ", inputFolder)
print("Dataset:", filename)

lineas = []
with open(fullFileName, 'r') as file:
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
	inFileRaster = inputFolder + i
	npI = np.fromfile(inFileRaster, dtype='int32')
	index = 0;
	for c in range(cols):
		for f in range(rows):
			tSeries[f][c][iFile] = int(npI[index])
			index += 1
	iFile += 1

valorMinimo = np.min(tSeries)
valorMaximo = np.max(tSeries)
valorPromedio = np.mean(tSeries)
valorMediana = np.median(tSeries)
desvEstandar = np.std(tSeries)
valorVarianza = np.var(tSeries)

nombre_archivo = outputFolder + filename[:-4] + ".stats.txt"
with open(nombre_archivo, 'w') as archivo:
	archivo.write
	archivo.write(f"Estadísticas Datos {filename}\n")
	archivo.write(f"\tMínimo: {valorMinimo}\n")
	archivo.write(f"\tMáximo: {valorMaximo}\n")
	archivo.write(f"\tPromedio: {valorPromedio}\n")
	archivo.write(f"\tMedia: {valorMediana}\n")
	archivo.write(f"\tDesviación estándar: {desvEstandar}\n")
	archivo.write(f"\tVarianza: {valorVarianza}\n")


uniDimData = tSeries.flatten()
frec, bordesBloques = np.histogram(uniDimData, bins=50)

# 4. Visualizar el histograma usando Matplotlib
plt.figure(figsize=(10, 6))
plt.hist(uniDimData, bins=bordesBloques, edgecolor='black')
plt.title('Histograma de los valores')
plt.xlabel('Valor')
plt.ylabel('Frecuencia')
plt.grid(axis='y', alpha=0.75)
nombreArchivoHistograma = outputFolder + filename[:-4] + ".histograma.png"
plt.savefig(nombreArchivoHistograma, bbox_inches='tight')
plt.close()

print("Frecuencias por bloque:", frec)
print("Bordes de los bloques:", bordesBloques)



