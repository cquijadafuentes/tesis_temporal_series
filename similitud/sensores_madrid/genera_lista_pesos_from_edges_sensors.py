import sys
import pandas as pd

# ------------------------ MAIN ------------------------
'''
	Este código lee los archivos con el mape entre sensores y edges, 
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
  sys.exit()

opcion = int(sys.argv[3])
if(opcion < 1 or opcion > 3):
	print("Error! la opción ingresada no es válida")
	print("\t<opcion> = 1 - Sensores en el mismo edge")
	print("\t<opcion> = 2 - Sensores a 1 edge de distancia")
	print("\t<opcion> = 3 - Sensores en el mismo edge y a 1 edge de distancia")
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
print("Cantidad de edges:", len(mapa_ES))

if(opcion == 1):
	# Buscando vecinos = sensores en el mismo edge
	sensores.sort()
	vecindad = {}
	for sensor in sensores:
		vecinos = []
		if sensor in mapa_SE:
			for edge in mapa_SE[sensor]:
				if edge in mapa_ES:
					for s in mapa_ES[edge]:
						if s != sensor and s not in vecinos:
							vecinos.append(s)
		if len(vecinos) > 0:
			vecinos.sort()
			vecindad[sensor] = vecinos
	sens_convecinos = sorted(vecindad)
	# Imprimiendo cantidad de sensores en la lista
	print(len(sens_convecinos))
	# Imprimiendo el ID de cada sensor en la lista
	for s in sens_convecinos:
		print(s, end=" ")
	print()
	# Por cada sensor, se imprime en una línea su ID, cantidad de vecinos, e ID de los vecinos.
	for sensor in sens_convecinos:
		lv = vecindad[sensor]
		print(sensor, len(lv), end=" ")
		for v in lv:
			print(v, end=" ")
		print()
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
	if edgeid not in mapa_EendN:
		mapa_EendN[edgeid] = [nodeEid]
	else:
		mapa_EendN[edgeid].append(nodeEid)
	# Agrega el edge de salida del start_node
	if(nodeSid not in mapa_NoutE):
		mapa_NoutE[nodeSid] = [edgeid]
	else:
		mapa_NoutE[nodeSid].append(edgeid)



# Buscando vecinos
sensores.sort()
vecindad = {}
for sensor in mapa_SE:
	print(sensor)

