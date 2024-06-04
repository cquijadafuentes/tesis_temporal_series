import sys
import pandas as pd

if len(sys.argv) != 4:
  print("Error! en la cantidad de argumentos.")
  print(sys.argv[0], "<sensordatafile.csv> <cant_muestras_por_sensor> <archivo_salida>")
  sys.exit()

entrada = sys.argv[1]
cant_muestras = int(sys.argv[2])
salida = sys.argv[3]

encabezados = ["id", "fecha", "tipo_elem", "intensidad", "ocupacion", "carga", "vmed", "error", "periodo_integracion"]
titulos = ["intensidad", "ocupacion", "carga", "vmed"]
df = pd.read_csv(entrada, sep=';', quotechar='"')
# df = pd.read_csv('/home/carlos/ubuntu_transfer/temporal_series/sensores_madrid/11-2023.csv', sep=';', quotechar='"')

print("Archivo: ", entrada)

print(df.info())
df_count = str(df.count())
filas = df.shape[0]

# Barrido para identificar cuántas filas cumplen la condición

cumplen = []
contador = 1;
indice = 1
while indice < filas:
	if(df['id'][indice] == df['id'][indice-1]):
		contador += 1
	else:
		if(contador >= cant_muestras):
			cumplen.append(df['id'][indice-1])
		contador = 1
	indice +=1
# Inserta último id de la lista si cumple condición.
if(contador >= cant_muestras):
	cumplen.append(df['id'][indice-1])

print("Se encontraron {} sensores con al menos {} muestras.".format(len(cumplen), cant_muestras))

with open(salida, 'w') as archivo:
	archivo.write("{}\t{}\n".format(len(cumplen), cant_muestras))
	i = 1
	archivo.write("{}".format(cumplen[0]))
	while i < len(cumplen):
		archivo.write("\t{}".format(cumplen[i]))
		i += 1
	archivo.write("\n")
	indice = 0
	for c in cumplen:
		# Encontrar la primera fila con la id correspondiente
		while(c != df['id'][indice]):
			indice += 1
		# Imprimir los valores en orden: "intensidad", "ocupacion", "carga", "vmed",
		for t in titulos:
			i = 1
			archivo.write("{}".format(df[t][indice]))
			while i < cant_muestras:
				archivo.write("\t{}".format(df[t][indice + i]))
				i += 1
			archivo.write("\n")
		# Incrementar índice en cant_muestras
		indice += cant_muestras

print("Fin")