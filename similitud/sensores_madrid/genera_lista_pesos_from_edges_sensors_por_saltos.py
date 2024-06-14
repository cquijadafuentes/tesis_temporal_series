import sys
import pandas as pd

def imprimir_salida(vecindad, outfile):
	sens_convecinos = sorted(vecindad)
	print("Generando archivo <{}> con la salida".format(outfile))
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
	sensor se usa una linea con el id del sensor, la cantidad de 
	vecinos y el id de los vecinos.

	Los vecinos de un sensor se definen por la cantidad de saltos 
	entre egdes. En este caso los sensores estan asociados a edges.
	Así mismo cada edge tiene mapeado los nodos que lo involucran.
	Un vecino se determina por el recorrido partiendo desde el edge
	del sensor y que se encuentra a una distancia menor o igual a la 
	cantidad de saltos que se ingresan en las opciones.
'''

if len(sys.argv) != 5:
  print("Error! en la cantidad de argumentos.")
  print(sys.argv[0], "<edge_sensor_map.csv> <edges.csv> <saltos> <output_file>")
  sys.exit()

saltos = int(sys.argv[3])
if saltos < 0:
	print("Error! la opción <saltos> ingresada no es válida")
	print("0 saltos son vecinos en el mismo edge.")
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

print("Cantidad de edges en el grafo:", len(mapa_EendN))

# Buscando vecinos 
sensores.sort()
vecindad = {}
print("Buscando vecinos...")
for sensor in sensores:
	edges_del_sensor = mapa_SE[sensor]
	indice = 0
	for s in range(saltos):
		cantidad = len(edges_del_sensor) - indice
		for c in range(cantidad):
			node = mapa_EendN[edges_del_sensor[indice]]
			if node in mapa_NoutE:
				listaOutterEdges = mapa_NoutE[node]
				for x in listaOutterEdges:
					if x not in edges_del_sensor:
						edges_del_sensor.append(x)
			indice += 1
	vecinos_sensor = []
	for x in edges_del_sensor:
		if x in mapa_ES:
			sensores_edge = mapa_ES[x]
			for s in sensores_edge:
				if ((s != sensor) and (s not in vecinos_sensor)):
					vecinos_sensor.append(s)
	if len(vecinos_sensor) > 0:
		vecinos_sensor.sort()
		vecindad[sensor] = vecinos_sensor
print("Se han encontrado {} sensores con vecinos.".format(len(vecindad)))
imprimir_salida(vecindad, sys.argv[4])
			