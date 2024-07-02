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

print("Archivo: ", sys.argv[1])

print("Contando NANs de sensores...")
nansIntensidad = 0
nansOcupacion = 0
nansCarga = 0
nansVmed = 0
nansSegsIntensidad = 0
nansSegsOcupacion = 0
nansSegsCarga = 0
nansSegsVmed = 0
filas = df.shape[0]
for x in range(filas):
	if(not df["intensidad"][x] >= 0):
		nansIntensidad += 1
		if( x != 1 and df['id'][x] == df['id'][x-1] and not (df["intensidad"][x] >= 0) and not (df["intensidad"][x-1] >= 0)):
			nansSegsIntensidad += 1
	if(not df["ocupacion"][x] >= 0):
		nansOcupacion += 1
		if( x != 1 and df['id'][x] == df['id'][x-1] and not (df["ocupacion"][x] >= 0) and not (df["ocupacion"][x-1] >= 0)):
			nansSegsOcupacion += 1
	if(not df["carga"][x] >= 0):
		nansCarga += 1
		if( x != 1 and df['id'][x] == df['id'][x-1] and not (df["carga"][x] >= 0) and not (df["carga"][x-1] >= 0)):
			nansSegsCarga += 1
	if(not df["vmed"][x] >= 0):
		nansVmed += 1
		if( x != 1 and df['id'][x] == df['id'][x-1] and not (df["vmed"][x] >= 0) and not (df["vmed"][x-1] >= 0)):
			nansSegsVmed += 1
print("Intensidad\n\t{} nans en total. {} nans seguidos.".format(nansIntensidad, nansSegsIntensidad))
print("Ocupacion\n\t{} nans en total. {} nans seguidos.".format(nansOcupacion, nansSegsOcupacion))
print("Carga\n\t{} nans en total. {} nans seguidos.".format(nansCarga, nansSegsCarga))
print("Vmed\n\t{} nans en total. {} nans seguidos.".format(nansVmed, nansSegsVmed))