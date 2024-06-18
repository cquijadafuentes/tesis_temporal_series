import sys
import matplotlib.pyplot as plt

'''
	Este código grafica las series de tiempo de los datos
	utilizando como entrada el mismo archivo .txt con el 
	que se calculan los índices de autocorrelación espacio
	temporal
	El formato del archivo es:
	Línea 1: cant_sensores	largo_muestras
	Línea 2: identificadores de los sensores
	Por cada sensor, 4 líneas con los datos de: 
	intensidad, ocupación, carga y velocidad media.

'''

def mostrar_menu():
	opc = -1
	cantopciones = 4
	while(opc < 0 or opc > cantopciones):
		print("***************** MENU *****************")
		print("1 - Mostrar ids de los sensores")
		print("2 - Imprimir datos de una serie de tiempo")
		print("3 - Imprimir comparación de dos series de tiempo")
		print("4 - Imprimir comparación de N series de tiempo")
		print("0 - Salir")
		print("****************************************")
		print("Ingrese una opcion...", end=" ")
		opc = int(input())
		if (opc < 0 or opc > cantopciones):
			print("La opción ingresada no es válida. Intente nuevamente...")
	return opc

if len(sys.argv) == 1:
	print("Error! en la cantidad de argumentos.")
	print(sys.argv[0], "<datafile.txt>")
	sys.exit()

titulos = ["intensidad", "ocupacion", "carga", "vmed"]
filename = sys.argv[1]
print("Archivo:", filename)
file = open(filename, "r")
# file = open('/home/carlos/ubuntu_transfer/temporal_series/sensores_madrid/data_magist/07-2023_porhora.744muestras.txt', "r")
print("Cargando datos...")
# Primera línea con cantidad de sensores y largo de las muestras
linea1 = file.readline().strip().split()
nSensores, nMuestras = int(linea1[0]), int(linea1[1])
print("Cantidad de sensores:", nSensores)
print("Largo de las muestras:", nMuestras)
# Segunda línea con identificadores de los sensores
ids = file.readline().strip().split()
# Siguientes líneas. Por cada sensor 4 líneas con los datos de
#	intensidad, ocupación, carga y velocidad media.
series = {}
for s in range(nSensores):
	dIntensidad = list(map(float, file.readline().strip().split()))
	dOcupacion = list(map(float, file.readline().strip().split()))
	dCarga = list(map(float, file.readline().strip().split()))
	dVmedia = list(map(float, file.readline().strip().split()))
	series[ids[s]] = [dIntensidad, dOcupacion, dCarga, dVmedia]
print("Datos cargados exitosamente")

opcion = 1
while(opcion != 0):
	opcion = mostrar_menu()
	if opcion == 1:
		# Mostrar identificadores de los sensores:
		print(ids)
	elif opcion == 2:
		# Graficar las 4 series de tiempo de un sensor:
		print("Ingrese el identificador del sensor")
		idtemp = input().strip()
		if(idtemp in series):
			sts = series[idtemp]
			fig, axs = plt.subplots(4, figsize=(10, 8))
			titulo = "Series de tiempo del sensor "+idtemp
			fig.suptitle(titulo, fontsize=14)
			for i in range(4):
				min_value = min(sts[i])
				max_value = max(sts[i])
				axs[i].plot(sts[i])
				axs[i].set_xlabel("Horas del mes")
				axs[i].set_ylabel(titulos[i])
				axs[i].set_yticks([min_value, max_value])
			# Ajustar el layout para que no se solapen las subfiguras
			plt.tight_layout()
			# Mostrar la figura
			plt.show()
			plt.close()
		else:
			print("El identificador del sensor ingresado no existe.")
	elif opcion == 3:
		# Graficar las series de tiempo de 2 sensores:
		print("Ingrese el identificador del 1er sensor")
		idtemp1 = input().strip()
		if idtemp1 in series:
			print("Ingrese el identificador del 2o sensor")
			idtemp2 = input().strip()
			if idtemp2 in series:
				sts1 = series[idtemp1]
				sts2 = series[idtemp2]
				fig, axs = plt.subplots(4, figsize=(10, 8))
				fig.suptitle("Comparación de dos series de tiempo", fontsize=16)
				for i in range(4):
					min_value = min(min(sts1[i]), min(sts2[i]))
					max_value = max(max(sts1[i]), max(sts2[i]))
					axs[i].plot(sts1[i], label=idtemp1)
					axs[i].plot(sts2[i], label=idtemp2)
					axs[i].set_title(titulos[i])
					axs[i].set_xlabel("Horas del mes")
					axs[i].set_ylabel(titulos[i])
					axs[i].set_yticks([min_value, max_value])
				plt.title("Comparación de series de tiempo")
				# Generar una leyenda general para la figura
				handles, labels = axs[0].get_legend_handles_labels()
				fig.legend(handles, labels, loc='upper right')
				# Ajustar el layout para que no se solapen las subfiguras
				plt.tight_layout()
				# Mostrar la figura
				plt.show()
				plt.close()
			else:
				print("El identificador del 2o sensor no existe.")
		else:
			print("El identificador del 1er sensor no existe.")
	elif opcion == 4:
		# Graficar las series de tiempo de N sensores:
		print("Ingrese los identificadores de los sensores en una línea, separados por espacio")
		stsIn = input().strip().split()
		sts = []
		for sin in stsIn:
			if sin in series:
				sts.append(sin)
		if len(sts) > 0:
			print(len(sts),"identificadores válidos encontrados")
			fig, axs = plt.subplots(4, figsize=(10, 8))
			fig.suptitle("Comparación de dos series de tiempo", fontsize=16)
			for i in range(4):
				min_value = series[sts[0]][i][0]
				max_value = min_value
				for j in sts:
					min_value = min(min_value, min(series[j][i]))
					max_value = max(max_value, max(series[j][i]))
					axs[i].plot(series[j][i], label=j)
				#axs[i].set_title(titulos[i])
				axs[i].set_xlabel("Horas del mes")
				axs[i].set_ylabel(titulos[i])
				axs[i].set_yticks([min_value, max_value])
			# Generar una leyenda general para la figura
			handles, labels = axs[0].get_legend_handles_labels()
			fig.legend(handles, labels, loc='lower right')
			# Ajustar el layout para que no se solapen las subfiguras
			plt.tight_layout()
			# Mostrar la figura
			plt.show()
			plt.close()
		else:
			print("No se encontraron identificadores válidos.")
print("FIN")