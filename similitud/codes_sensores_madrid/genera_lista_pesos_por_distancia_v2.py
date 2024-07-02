import sys
import pandas as pd
from geopy import distance

# ------------------------ MAIN ------------------------
'''
	Este código lee el archivo con la ubicación de los sensores
	de tráfico de la red de transporte de madrid y junto con un
	valor de distancia se genera la matriz de pesos
	Se imprimen primero la cantidad de sensores, seguido por los
	identificadores de cada senor y finalmente lista donde por cada
	sensor se muestran la cantidad de sensores vecinos.
'''

if len(sys.argv) != 3:
  print("Error! en la cantidad de argumentos.")
  print(sys.argv[0], "<ubicacion_sensores.csv> <distancia_minima_km>")
  sys.exit()

umbral = float(sys.argv[2])

encabezados = ['tipo_elem', 'distrito', 'id', 'cod_cent', 'nombre', 'utm_x', 'utm_y', 'longitud', 'latitud']
datos = pd.read_csv(sys.argv[1], sep=';', quotechar='"')
# datos = pd.read_csv('/home/carlos/tesis_temporal_series/similitud/sensores_madrid/datos/pmed_ubicacion_02-2024.csv', sep=';', quotechar='"')
datos = datos.sort_values('id')
filas = datos.shape[0]

listaVecinos = {}

for i in range(filas):
	cooI = (datos['latitud'][i],datos['longitud'][i])
	lista = []
	for j in range(filas):
		cooJ = (datos['latitud'][j],datos['longitud'][j])
		if(datos['id'][i] != datos['id'][j]):
			dKm = distance.distance(cooI, cooJ).km
			if(dKm <= umbral):
				lista.append(datos['id'][j])
	if len(lista) > 0:
		lista.sort()
		listaVecinos[datos['id'][i]] = lista

listaIds = sorted(listaVecinos)
print(len(listaIds))
for id in listaIds:
	print(id, end="\t")

for id in listaIds:
	if id in listaVecinos:
		print(id, end="\t")
		vecinos = listaVecinos[id]
		print(len(vecinos), end="\t")
		for v in vecinos:
			if v in listaVecinos:
				print(v, end="\t")
		print("")


