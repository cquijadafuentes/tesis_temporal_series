import sys
import pandas as pd
import matplotlib.pyplot as plt

def mostrar_grafica(datos, titulo, labelx, labely):
	fig, ax = plt.subplots()
	plt.plot(datos)
	plt.title(titulo)
	ax.set_xlabel(labelx)
	ax.set_ylabel(labely)
	ax.grid(True)
	fig.set_size_inches(12, 4)
	plt.tight_layout()
	plt.show()
	plt.close()

if len(sys.argv) != 3:
	print("Error! en la cantidad de argumentos.")
	print(sys.argv[0], "<edge_sensor_map.csv> <edges.csv>")

data_ES = pd.read_csv(sys.argv[1], sep=',', quotechar='"')
paresEdgeSensor = data_ES.shape[0]
sensores = []

# Mapas entre sensores y edges
mapa_ES = {}
mapa_SE = {}
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

edges_por_sensor = []
sensores_dosomasedges = []
for x in mapa_SE:
	l = len(mapa_SE[x])
	edges_por_sensor.append(l)
	if l > 1:
		sensores_dosomasedges.append([x,l])
edges_por_sensor.sort()
mostrar_grafica(edges_por_sensor, "Histograma: edges por sensor", str(len(mapa_SE)) + " sensores.", "Cantidad de edges")

sensores_por_edge = []
edges_dosomassensores = []
for x in mapa_ES:
	l = len(mapa_ES[x])
	sensores_por_edge.append(l)
	if l > 1:
		edges_dosomassensores.append([x,l])
sensores_por_edge.sort()
mostrar_grafica(sensores_por_edge, "Histograma: sensores por edge", str(len(mapa_ES)) + " edges con sensor.", "Cantidad de sensores")

print("sensores:", len(sensores))
print("sensores con edges:", len(mapa_SE))
print("edges con sensores:", len(mapa_ES))
################################################################################
################# Buscando vecinos = sensores en el mismo edge #################
################################################################################
sensores.sort()
vecindad0 = {}
vecinos_enmismoedge = []
cant_vecinos_enmismoedge = []
for sensor in mapa_SE:
	vecinos = []
	for edge in mapa_SE[sensor]:
		if edge in mapa_ES:
			for s in mapa_ES[edge]:
				if s != sensor and s not in vecinos:
					vecinos.append(s)
	if len(vecinos) > 0:
		vecinos_enmismoedge.append([sensor, len(vecinos)])
		cant_vecinos_enmismoedge.append(len(vecinos))
		vecindad0[sensor] = vecinos

cant_vecinos_enmismoedge.sort()
mostrar_grafica(cant_vecinos_enmismoedge, "Histograma: vecinos en el mismo edge", str(len(vecinos_enmismoedge)) + " sensores con vecinos.", "Cantidad de vecinos")

print("sensores con vecinos en el mismo edge:", len(cant_vecinos_enmismoedge))

################################################################################
############### Buscando vecinos = sensores en un edge siguiente ###############
################################################################################

data_EN = pd.read_csv(sys.argv[2], sep=',', quotechar='"')

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

cantidad_outteredges = []
for x in mapa_NoutE:
	cantidad_outteredges.append(len(mapa_NoutE[x]))

cantidad_outteredges.sort()
mostrar_grafica(cantidad_outteredges, "Histograma: edges de salida de los nodos", str(len(mapa_NoutE))+" nodos totales", "Cantidad de edges de salida")

cant_vecinos_ensiguienteedge = []
# Buscando vecinos a distancia 1 edge
vecindad1 = {}
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
		vecindad1[sensor] = vecinos
		cant_vecinos_ensiguienteedge.append(len(vecinos))

print("sensores con vecinos en el edge siguiente:", len(cant_vecinos_ensiguienteedge), len(vecindad1))

cant_vecinos_ensiguienteedge.sort()
mostrar_grafica(cant_vecinos_ensiguienteedge, "Histograma: vecinos en edge siguiente", str(len(cant_vecinos_ensiguienteedge)) + " sensores con vecinos.", "Cantidad de vecinos")

################################################################################
########################### Buscando vecinos unidos ###########################
################################################################################


idsVME = sorted(vecindad0)
for sensor in idsVME:
	if sensor not in vecindad1:
		vecindad1[sensor] = vecindad0[sensor]
	else:
		for v in vecindad0[sensor]:
			if v not in vecindad1[sensor]:
				vecindad1[sensor].append(v)

cant_vecindad1 = []
for x in vecindad1:
	cant_vecindad1.append(len(vecindad1[x]))

print("sensores con vecinos en mismo edge + edge siguiente", len(cant_vecindad1))
cant_vecindad1.sort()
mostrar_grafica(cant_vecindad1, "Histograma: vecinos unidos", str(len(cant_vecindad1)) + " sensores con vecinos.", "Cantidad de vecinos")


################################################################################
########################## Buscando vecinos desunidos ##########################
################################################################################

for sensor in idsVME:
	for v in vecindad0[sensor]:
		if v in vecindad1[sensor]:
			vecindad1[sensor].remove(v)
			if(len(vecindad1[sensor]) == 0):
				del vecindad1[sensor]

cant_vecindad1 = []
for x in vecindad1:
	cant_vecindad1.append(len(vecindad1[x]))
print("sensores con vecinos en edge siguiente sin los vecinos del mismo edge", len(cant_vecindad1))

cant_vecindad1.sort()
mostrar_grafica(cant_vecindad1, "Histograma: vecinos desunidos", str(len(cant_vecindad1)) + " sensores con vecinos.", "Cantidad de vecinos")

################################################################################
####################	Listas con detalle de información	####################
################################################################################

print("Lista de {} sensores con más de un edge asociado:".format(len(sensores_dosomasedges)))
sensores_dosomasedges = sorted(sensores_dosomasedges, key=lambda x : (x[1],x[0]))
for x in sensores_dosomasedges:
	print("{} tiene {} edges".format(x[0], x[1]))

print("Lista de {} edges con más de un sensor asociado:".format(len(edges_dosomassensores)))
edges_dosomassensores = sorted(edges_dosomassensores, key=lambda x : (x[1],x[0]))
for x in edges_dosomassensores:
	print("{} tiene {} sensores".format(x[0], x[1]))

print("Lista de {} sensores con vecinos en el mismo edge:".format(len(vecinos_enmismoedge)))
vecinos_enmismoedge = sorted(vecinos_enmismoedge, key=lambda x : (x[1],x[0]))
for x in vecinos_enmismoedge:
	print("{} tiene {} vecinos en el mismo edge".format(x[0], x[1]))
