import sys
import matplotlib.pyplot as plt

'''
	Este archivo genera un histograma con la cantidad de vecinos que hay por sensor
	Recibe como argumento archivos de lista de vecindad en el siguiente formato:
	primera linea: cantidad de sensores
	segunda linea: ids separados por espacio de cada sensor
	siguientes líneas: por cada sensor una línea donde se indica, separados por espacio
		lo siguiente:
			- id del sensor
			- cantidad de vecinos del sensor
			- ids de los vecinos del sensor.
	En total el archivo tendrá <cant_sensores> + 2 líneas con información.
'''

if len(sys.argv) == 1:
	print("Error! en la cantidad de argumentos.")
	print(sys.argv[0], "<lista_1> [<lista_2> [<lista_3> [...]]]")
	sys.exit()

num_files = int(len(sys.argv)) - 1
# Preparando entorno para gráficas:
fig, ax = plt.subplots()

print("Cantidad de archivos:", num_files)
for nf in range(num_files):
	
	# Cargando archivo
	filename = sys.argv[nf+1]
	print("Archivo:", filename)
	file = open(filename, "r")
	# La primera línea indica la cantidad de sensores
	line = file.readline().strip()
	print(line)
	sensores = int(line)
	# La segunda línea posee los ids de los sensores
	line = file.readline().strip()
	# Cada linea posee el id, la cantidad de vecinos y los ids de los vecinos.
	datos = []
	for s in range(sensores):
		line = file.readline().strip().split()
		cant_vecinos = int(line[1])
		datos.append(cant_vecinos)
	file.close()
	datos.sort()
	plt.plot(datos, label=filename)
print("fin")

plt.title("Histograma vecindades")
ax.set_xlabel("Sensores")
ax.set_ylabel("Cant. de vecinos")
ax.legend()
ax.grid(True)
fig.set_size_inches(12, 4)
plt.tight_layout()
plt.show()
plt.close()
