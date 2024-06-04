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

if len(sys.argv) != 2:
	print("Error! en la cantidad de argumentos.")
	print(sys.argv[0], "<edge_sensor_map.csv>")

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

# Buscando vecinos = sensores en el mismo edge
sensores.sort()
vecindad = {}
vecinos_enmismoedge = []
cant_vecinos_enmismoedge = []
for sensor in sensores:
	vecinos = []
	if sensor in mapa_SE:
		for edge in mapa_SE[sensor]:
			if edge in mapa_ES:
				for s in mapa_ES[edge]:
					if s != sensor and s not in vecinos:
						vecinos.append(s)
	if len(vecinos) > 0:
		vecinos_enmismoedge.append([sensor, len(vecinos)])
		cant_vecinos_enmismoedge.append(len(vecinos))

cant_vecinos_enmismoedge.sort()
mostrar_grafica(cant_vecinos_enmismoedge, "Histograma: vecinos en el mismo edge", str(len(vecinos_enmismoedge)) + " sensores con vecinos.", "Cantidad de vecinos")

print("sensores con vecinos en el mismo edge:", len(cant_vecinos_enmismoedge))

# Buscando vecinos = sensores en un edge siguiente


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
