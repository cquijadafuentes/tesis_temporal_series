import sys
import pandas as pd

def imprimir_salida(vecindad, outfile):
	sens_convecinos = sorted(vecindad)
	print("Generando archivo {} con la salida".format(outfile))
	salida = open(outfile, "w")
	salida.write(str(len(sens_convecinos))+"\n")
	# Imprimiendo el ID de cada sensor en la lista
	outline = ""
	for s in sens_convecinos:
		outline += str(s) + " "
	salida.write(outline+"\n")
	# Por cada sensor, se imprime en una línea su ID, cantidad de vecinos, e ID de los vecinos.
	for sensor in sens_convecinos:
		lv = vecindad[sensor]
		outline = str(sensor) + " " + str(len(lv))
		for v in lv:
			outline += " " + str(v)
		salida.write(outline+"\n")
	salida.close()

# ------------------------ MAIN ------------------------
'''
	Este código lee los archivos con el mapeo entre sensores y edges, 
	y el mapeo entre edges y nodes generados en magist.

	Se imprimen primero la cantidad de sensores, seguido por los
	identificadores de cada senor y finalmente lista donde por cada
	sensor se muestran la cantidad de sensores vecinos.
'''

if len(sys.argv) != 5:
  print("Error! en la cantidad de argumentos.")
  print(sys.argv[0], "<edge_sensor_map.csv> <edges.csv> <opcion> <output_file>")
  print("\t<opcion> = 1 - Sensores en el mismo edge")
  print("\t<opcion> = 2 - Sensores a 1 edge de distancia")
  print("\t<opcion> = 3 - Sensores en el mismo edge y a 1 edge de distancia")
  print("\t<opcion> = 4 - Sensores en el edge siguiente sin los del mismo edge")
  sys.exit()

opcion = int(sys.argv[3])
if(opcion < 1 or opcion > 4):
	print("Error! la opción ingresada no es válida")
	print("\t<opcion> = 1 - Sensores en el mismo edge")
	print("\t<opcion> = 2 - Sensores a 1 edge de distancia")
	print("\t<opcion> = 3 - Sensores en el mismo edge y a 1 edge de distancia")
	print("\t<opcion> = 4 - Sensores en el edge siguiente sin los del mismo edge")
	sys.exit()

# Datos Edge Sensor
data_ES = pd.read_csv(sys.argv[1], sep=',', quotechar='"')
# data_ES = pd.read_csv('/home/carlos/ubuntu_transfer/temporal_series/sensores_madrid/data_magist/edge_sensor_map.csv', sep=',', quotechar='"')
# Datos Edge Nodes
data_EN = pd.read_csv(sys.argv[2], sep=',', quotechar='"')
# data_EN = pd.read_csv('/home/carlos/ubuntu_transfer/temporal_series/sensores_madrid/data_magist/edges.csv', sep=',', quotechar='"')

sensores = []

# Mapas entre sensores y edges
mapa_ES = {}
mapa_SE = {}
paresEdgeSensor = data_ES.shape[0]
for x in range(paresEdgeSensor):
	# Los encabezados son 'edge_id' y 'sensor_id'
	edgeid = int(data_ES['edge_id'][x])
	sensorid = int(data_ES['sensor_id'][x])
	# Agregando al mapa edges-sensors
	if(edgeid not in mapa_ES):
		mapa_ES[edgeid] = [sensorid]
	else:
		mapa_ES[edgeid].append(sensorid)
	# Agregando al mapa sensors-edges
	if(sensorid not in mapa_SE):
		mapa_SE[sensorid] = [edgeid]
	else:
		mapa_SE[sensorid].append(edgeid)
	# Agregando sensor a la lista
	if(sensorid not in sensores):
		sensores.append(sensorid)

print("Cantidad de sensores:", len(mapa_SE))
print("Cantidad de edges con sensor:", len(mapa_ES))
sensores.sort()

if(opcion == 1):
	# Buscando vecinos = sensores en el mismo edge
	print("Buscando vecinos = sensores en el mismo edge...")
	vecindad = {}
	for sensor in mapa_SE:
		vecinos = []
		for edge in mapa_SE[sensor]:
			if edge in mapa_ES:
				for s in mapa_ES[edge]:
					if s != sensor and s not in vecinos:
						vecinos.append(s)
		if len(vecinos) > 0:
			vecinos.sort()
			vecindad[sensor] = vecinos
	print("{} sensores con vecinos en el mismo edge".format(len(vecindad)))
	# Imprimiendo cantidad de sensores en la lista
	imprimir_salida(vecindad, sys.argv[4])
	sys.exit()

# Mapas entre edges y nodos
mapa_EendN = {}		# por cada edge se indica el end-node	(1-1)
mapa_NoutE = {}		# por cada node se indican los outter edges 	(1-n)
filasEdgeNode = data_EN.shape[0]
for x in range(filasEdgeNode):
	# Los encabezados son 'id' (edge), 'start_node' y 'end_node'
	edgeid = data_EN['id'][x]
	nodeSid = data_EN['start_node'][x]
	nodeEid = data_EN['end_node'][x]
	# Inserta el end_node de cada edge
	mapa_EendN[edgeid] = nodeEid
	# Agrega el edge de salida del start_node
	if(nodeSid not in mapa_NoutE):
		mapa_NoutE[nodeSid] = [edgeid]
	else:
		mapa_NoutE[nodeSid].append(edgeid)

print("Cantidad de edges:", len(mapa_EendN))

# Buscando vecinos a distancia 1 edge
vecinosEdgeSig = {}
for sensor in sensores:
	vecinos = []
	listaEdges = mapa_SE[sensor]
	for edgeId in listaEdges:
		endNodeId = mapa_EendN[edgeId]
		listOutterEdges = mapa_NoutE[endNodeId]
		for outterEdge in listOutterEdges:
			if outterEdge in mapa_ES:
				listaSensoresEdge = mapa_ES[outterEdge]
				for sensorVecino in listaSensoresEdge:
					if sensorVecino not in vecinos:
						vecinos.append(sensorVecino)
	if len(vecinos) > 0:
		vecinos.sort()
		vecinosEdgeSig[sensor] = vecinos
print("{} sensores con vecinos encontrados en edge siguiente".format(len(vecinosEdgeSig)))

if(opcion == 2):
	imprimir_salida(vecinosEdgeSig, sys.argv[4])
	sys.exit()

# Vecinos en el mismo edge para aplicar en opciones 3 y 4
vecinosMismoEdge = {}
for sensor in mapa_SE:
	vecinos = []
	for edge in mapa_SE[sensor]:
		if edge in mapa_ES:
			for s in mapa_ES[edge]:
				if s != sensor and s not in vecinos:
					vecinos.append(s)
	if len(vecinos) > 0:
		vecinos.sort()
		vecinosMismoEdge[sensor] = vecinos
print("{} sensores con vecinos en el mismo edge".format(len(vecinosMismoEdge)))

# Opción = 3: se deben combinar los resultados antes de imprimirlos
if(opcion == 3):
	vecinosUnidos = vecinosEdgeSig
	idsVecinosMismoEdge = sorted(vecinosMismoEdge)
	for sensor in idsVecinosMismoEdge:
		if sensor not in vecinosUnidos:
			vecinosUnidos[sensor] = vecinosMismoEdge[sensor]
		else:
			listaVME = vecinosMismoEdge[sensor]
			for v in listaVME:
				if v not in vecinosUnidos[sensor]:
					vecinosUnidos[sensor].append(v)
			vecinosUnidos[sensor].sort()
	print("{} sensores con vecinos en mismo edge + edge siguiente".format(len(vecinosUnidos)))
	imprimir_salida(vecinosUnidos, sys.argv[4])
	sys.exit()

# Opción = 4: se deben quitar los vecinos con el mismo edge desde la lista con vecinos de 1 edge de distancia
if(opcion == 4):
	vecinosDesunidos = vecinosEdgeSig
	idsVecinosMismoEdge = sorted(vecinosMismoEdge)
	for sensor in idsVecinosMismoEdge:
		if sensor in vecinosDesunidos:
			listaVME = vecinosMismoEdge[sensor]
			for v in listaVME:
				if v in vecinosDesunidos[sensor]:
					vecinosDesunidos[sensor].remove(v)
					if(len(vecinosDesunidos[sensor]) == 0):
						del vecinosDesunidos[sensor]
	print("{} sensores con vecinos en edge siguiente - vecinos en mismo edge".format(len(vecinosDesunidos)))
	imprimir_salida(vecinosDesunidos, sys.argv[4])
	sys.exit()
