import sys
import pandas as pd

# --------------------- FUNCIONES ---------------------

def print_menu():
	print("****************************")
	print("1 - Mostrar head()")
	print("2 - Mostrar info()")
	print("3 - Mostrar count()")
	print("4 - Mostrar fila N")
	print("5 - Mostrar las N primeras filas")
	print("6 - Mostrar filas de la N a la M")
	print("7 - Mostrar las N primeras filas con error")
	print("8 - Cantidad ids distintas")
	print("9 - Mostrar las ids")
	print("10 - Mostrar las ids y cantidades")
	print("11 - Contar nans de datos")
	print("12 - Mostrar datos de un sensor")

	print("0 - Salir")
	print("****************************")

# --------------------- FUNCIONES ---------------------

def print_fila(df, enc):
	print(len(df), " filas con datos")
	print("Ingrese el id de la fila: ")
	f = int(input())
	for e in enc:
		print("{}\t{}".format(e, df[e][f]))

# --------------------- FUNCIONES ---------------------

def print_filas(df, enc):
	print(len(df), " filas con datos")
	print("Ingrese el id de la fila N: ")
	N = len(df)
	f1 = int(input())
	if (f1 >= N):
		f1 = N-1
	print("Ingrese el id de la fila M: ")
	f2 = int(input())
	if (f2 >= N):
		f2 = N-1
	if (f1 > f2):
		f1,f2 = f2,f1
	for e in enc:
		print(e, end="\t")
	print()
	for i in range(f1, f2+1):
		for e in enc:
			print(df[e][i], end="\t")
		print()
	print()

# --------------------- FUNCIONES ---------------------

def print_primerasfilas(df, enc):
	print(len(df), " filas con datos")
	if(len(df) == 0):
		print("No hay filas que mostrar.")
		return
	print(len(df), "filas en los datos.")
	print("Ingrese la cantidad de filas a mostrar: ")
	N = int(input())
	if (N >= len(df)):
		N = len(df)-1
	for e in enc:
		print(e, end="\t")
	print()
	for i in range(N):
		for e in enc:
			print(df[e][i], end="\t")
		print()
	print()

# --------------------- FUNCIONES ---------------------

def encuentra_ids(df):
	dic_ids = {}
	for e in df['id']:
		if(e not in dic_ids):
			dic_ids[e] = 1
		else:
			dic_ids[e] += 1
	return dic_ids

# --------------------- FUNCIONES ---------------------

def encuentra_errores(df):
	lista_errores = []
	indice = 0
	for e in df['error']:
		if(e != 'N'):
			lista_errores.append(indice)
		indice += 1
	return lista_errores

# --------------------- FUNCIONES ---------------------

def muestra_errores(df, errores, enc):
	if(len(errores) == 0):
		print("No hay errores en la lista")
		return
	print("Se han encontrado {} errores en la lista".format(len(errores)))
	print("Ingrese la cantidad de filas a mostrar:")
	N = int(input())
	if(len(errores)<N):
		N = len(errores)
		print("Existen menos errores del valor ingresado")
	for e in enc:
		print(e, end="\t")
	print()
	for i in range(N):
		for e in enc:
			print(df[e][i], end="\t")
		print()
	print()

# --------------------- FUNCIONES ---------------------

def contar_nans(df, df_encabezados):
	print("Contando NANs de sensores")
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
			if( x != 1 and df['id'][x] == df['id'][x-1] and df["intensidad"][x] == df["intensidad"][x-1]):
				nansSegsIntensidad += 1
		if(not df["ocupacion"][x] >= 0):
			nansOcupacion += 1
			if( x != 1 and df['id'][x] == df['id'][x-1] and df["ocupacion"][x] == df["ocupacion"][x-1]):
				nansSegsOcupacion += 1
		if(not df["carga"][x] >= 0):
			nansCarga += 1
			if( x != 1 and df['id'][x] == df['id'][x-1] and df["carga"][x] == df["carga"][x-1]):
				nansSegsCarga += 1
		if(not df["vmed"][x] >= 0):
			nansVmed += 1
			if( x != 1 and df['id'][x] == df['id'][x-1] and df["vmed"][x] == df["vmed"][x-1]):
				nansSegsVmed += 1
	print("Intensidad\n\t{} nans en total. {} nans seguidos.".format(nansIntensidad, nansSegsIntensidad))
	print("Ocupacion\n\t{} nans en total. {} nans seguidos.".format(nansOcupacion, nansSegsOcupacion))
	print("Carga\n\t{} nans en total. {} nans seguidos.".format(nansCarga, nansSegsCarga))
	print("Vmed\n\t{} nans en total. {} nans seguidos.".format(nansVmed, nansSegsVmed))


# --------------------- FUNCIONES ---------------------

def mostrar_datos_sensor(df, dic_ids, df_encabezados):
	print("Ingrese id del sensor")
	sens = int(input())
	if sens not in dic_ids:
		print("Error! el id ingresado no corresponde.")
		return
	filtrado = df[df['id'] == sens]
	for y, x in filtrado.iterrows():
		for z in df_encabezados:
			print(x[z], end='\t')
		print("")
	print("{} filas en total.".format(filas))

# ------------------------ MAIN ------------------------

if len(sys.argv) != 2:
  print("Error! en la cantidad de argumentos.")
  print(sys.argv[0], "<sensordatafile.csv>")
  sys.exit()

df_encabezados = ["id", "fecha", "tipo_elem", "intensidad", "ocupacion", "carga", "vmed", "error", "periodo_integracion"]
df = pd.read_csv(sys.argv[1], sep=';', quotechar='"')
# df = pd.read_csv('/home/carlos/ubuntu_transfer/temporal_series/sensores_madrid/11-2023.csv', sep=';', quotechar='"')
# df = pd.read_csv('/home/carlos/ubuntu_transfer/temporal_series/sensores_madrid/data_magist/07-2023.csv', sep=';', quotechar='"')

print("Archivo: ", sys.argv[1])
df_count = str(df.count())

print(df.head())
print(df.info())
print(df_count)
lista_errores = encuentra_errores(df)
dic_ids = encuentra_ids(df)
filas = df.shape[0]

opc = -1
print_menu()
opc = int(input())
while(opc != 0):
	if(opc == 1):
		print("Mostrando head")
		print(df.head())
	elif(opc == 2):
		print("Mostrando info")
		print(df.info())
	elif(opc == 3):
		print("Mostrando count")
		print(df_count)
	elif(opc == 4):
		print("Mostrando fila N")
		print_fila(df, df_encabezados)
	elif(opc == 5):
		print("Mostrando N primeras filas")
		print_primerasfilas(df, df_encabezados)
	elif(opc == 6):
		print("Mostrando fila N")
		print_filas(df, df_encabezados)
	elif(opc == 7):
		print("Mostrando filas con error")
		muestra_errores(df, lista_errores, df_encabezados)
	elif(opc == 8):
		print("Cantidad de ids diferentes:", len(dic_ids))
	elif(opc == 9):
		print("Hay {} ids diferentes".format(len(dic_ids)))
		print(sorted(dic_ids))
	elif(opc == 10):
		print("Hay {} ids diferentes".format(len(dic_ids)))
		for x in sorted(dic_ids):
			print("{} - {}".format(x, dic_ids[x]), end="\t")
		print(" ")
	elif(opc == 11):
		contar_nans(df, df_encabezados)
	elif(opc == 12):
		print("Cantidad de ids diferentes:", len(dic_ids))
		mostrar_datos_sensor(df, dic_ids, df_encabezados)


	else:
		print("Opción no disponible... intentar de nuevo.")
	print_menu()
	opc = int(input())
print("Fin")