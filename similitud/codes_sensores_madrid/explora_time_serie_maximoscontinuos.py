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

print("Archivo: ", sys.argv[1])

print("Contando sensores...")
muestrasIntensidad = 1
muestrasOcupacion = 1
muestrasCarga = 1
muestrasVmed = 1
maximoIntensidad = 0
maximoOcupacion = 0
maximoCarga = 0
maximoVmed = 0
continuoIntensidad = 1
continuoOcupacion = 1
continuoCarga = 1
continuoVmed = 1

percentil = filas / 100
porcentaje = 0
for x in range(100):
	print("|",end="")
print()

for x in range(1,filas):
	if porcentaje*percentil < x:
		print("|",end="")
		porcentaje += 1
	if(df['id'][x] == df['id'][x-1]):
		if(df["intensidad"][x] >= 0):
			muestrasIntensidad += 1
		else:
			if not df["intensidad"][x-1] >= 0 :
				continuoIntensidad += 1
			else:
				if continuoIntensidad > maximoIntensidad:
					maximoIntensidad = continuoIntensidad
				continuoIntensidad = 1
		if(df["ocupacion"][x] >= 0):
			muestrasOcupacion += 1
		else:
			if not df["ocupacion"][x-1] >= 0 :
				continuoOcupacion += 1
			else:
				if continuoOcupacion > maximoOcupacion:
					maximoOcupacion = continuoOcupacion
				continuoOcupacion = 1
		if(df["carga"][x] >= 0):
			muestrasCarga += 1
		else:
			if not df["carga"][x-1] >= 0 :
				continuoCarga += 1
			else:
				if continuoCarga > maximoCarga:
					maximoCarga = continuoCarga
				continuoCarga = 1
		if(df["vmed"][x] >= 0):
			muestrasVmed += 1
		else:
			if not df["vmed"][x-1] >= 0 :
				continuoVmed += 1
			else:
				if continuoVmed > maximoVmed:
					maximoVmed = continuoVmed
				continuoVmed = 1
print("")
if continuoIntensidad > maximoIntensidad:
	maximoIntensidad = continuoIntensidad
if continuoOcupacion > maximoOcupacion:
	maximoOcupacion = continuoOcupacion
if continuoCarga > maximoCarga:
	maximoCarga = continuoCarga
if continuoVmed > maximoVmed:
	maximoVmed = continuoVmed

print("Maximo continuo de Intensidad: ", maximoIntensidad)
print("Maximo continuo de Ocupacion: ", maximoOcupacion)
print("Maximo continuo de Carga: ", maximoCarga)
print("Maximo continuo de Vmed: ", maximoVmed)