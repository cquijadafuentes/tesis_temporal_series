import sys
import pandas as pd

if len(sys.argv) != 2:
  print("Error! en la cantidad de argumentos.")
  print(sys.argv[0], "<sensordatafile.csv>")
  sys.exit()

df_encabezados = ["id", "fecha", "tipo_elem", "intensidad", "ocupacion", "carga", "vmed", "error", "periodo_integracion"]
df = pd.read_csv(sys.argv[1], sep=';', quotechar='"')
# df = pd.read_csv('/home/carlos/ubuntu_transfer/temporal_series/sensores_madrid/11-2023.csv', sep=';', quotechar='"')
# df = pd.read_csv('/home/carlos/ubuntu_transfer/temporal_series/sensores_madrid/data_magist/07-2023.csv', sep=';', quotechar='"')
filas = df.shape[0]

#print("Archivo: ", sys.argv[1])

#print("Contando sensores...")
muestrasIntensidad = 1
muestrasOcupacion = 1
muestrasCarga = 1
muestrasVmed = 1

lista_mayor_muestras = []
cant_muestras = 2800
# Revisando elementos con al menos cant_muestras
for x in range(1,filas):
	if(df['id'][x] == df['id'][x-1]):
		if(df["intensidad"][x] >= 0):
			muestrasIntensidad += 1
		if(df["ocupacion"][x] >= 0):
			muestrasOcupacion += 1
		if(df["carga"][x] >= 0):
			muestrasCarga += 1
		if(df["vmed"][x] >= 0):
			muestrasVmed += 1
	else:
		if(muestrasIntensidad > cant_muestras and muestrasOcupacion > cant_muestras and muestrasCarga > cant_muestras and muestrasVmed > cant_muestras):
			lista_mayor_muestras.append(df['id'][x-1])
if(muestrasIntensidad > cant_muestras and muestrasOcupacion > cant_muestras and muestrasCarga > cant_muestras and muestrasVmed > cant_muestras):
	lista_mayor_muestras.append(df['id'][filas-1])

infoAcum = ["-", "-"]
datosAcum = [0, 0, 0, 0]
cantAcu = [0, 0, 0 , 0]

print('"id";"fecha";"intensidad";"ocupacion";"carga";"vmed"')
# Promediando los valores por la fecha-hora
for x in range(filas):
	# SI: el id tiene más de cant_muestras, no es la primera fila del archivo y no es el primer registro con el mismo id y la misma fecha-hora
	if(df['id'][x] in lista_mayor_muestras and x>0 and df['id'][x] == df['id'][x-1] and df['fecha'][x][:13] == infoAcum[1]):
		# Acumular valores
		if (df['intensidad'][0] >= 0):
			datosAcum[0] += df['intensidad'][x]
			cantAcu[0] += 1
		if (df['ocupacion'][x] >= 0):
			datosAcum[1] += df['ocupacion'][x]
			cantAcu[1] += 1
		if (df['carga'][x] >= 0):
			datosAcum[2] += df['carga'][x]
			cantAcu[2] += 1
		if (df['vmed'][x] >= 0):
			datosAcum[3] += df['vmed'][x]
			cantAcu[3] += 1
	else:
		# SI: el id de la fila anterior si estaba en la lista se deben mostrar sus datos
		if(x > 0 and df['id'][x-1] in lista_mayor_muestras):
			# Calcular promedio valores anteriores
			for i in range(4):
				if(cantAcu[i] > 0):
					datosAcum[i] = datosAcum[i] / cantAcu[i]
				else:
					datosAcum[i] = "NaN"
			#	Mostrar valores anteriores
			print('{};"{}";"{}";"{}";"{}";"{}"'.format(infoAcum[0], infoAcum[1], datosAcum[0], datosAcum[1], datosAcum[2], datosAcum[3]))
			infoAcum = ["-", "-"]
			datosAcum = [0, 0, 0, 0]
			cantAcu = [0, 0, 0 , 0]
		# SI: este corresponde a la primera fila con una id que esta en la lista
		if(df['id'][x] in lista_mayor_muestras):
			# Resetear valores para siguientes cálculos
			infoAcum = [df['id'][x], df['fecha'][x][:13]]
			datosAcum = [0, 0, 0, 0]
			cantAcu = [0, 0, 0 , 0]
			if (df['intensidad'][x] >= 0):
				datosAcum[0] +=df['intensidad'][x]
				cantAcu[0] += 1
			if (df['ocupacion'][x] >= 0):
				datosAcum[1] += df['ocupacion'][x]
				cantAcu[1] += 1
			if (df['carga'][x] >= 0):
				datosAcum[2] += df['carga'][x]
				cantAcu[2] += 1
			if (df['vmed'][x] >= 0):
				datosAcum[3] += df['vmed'][x]
				cantAcu[3] += 1
# Repetir instrucciones que muestran el último promedio
if (df['id'][filas-1] in lista_mayor_muestras):
	# Calcular promedio con los valores guardados
	for i in range(4):
		if(cantAcu[i] > 0):
			datosAcum[i] = datosAcum[i] / cantAcu[i]
		else:
			datosAcum[i] = "NaN"
	#	Mostrar valores anteriores
	print('{};"{}";"{}";"{}";"{}";"{}"'.format(infoAcum[0], infoAcum[1], datosAcum[0], datosAcum[1], datosAcum[2], datosAcum[3]))
