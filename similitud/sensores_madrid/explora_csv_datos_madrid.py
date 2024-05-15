import sys
import pandas as pd

def print_menu():
	print("****************************")
	print("1 - Mostrar head()")
	print("2 - Mostrar info()")
	print("3 - Mostrar count()")
	print("4 - Mostrar fila N")
	print("5 - Mostrar filas de la N a la M")

	print("0 - Salir")
	print("****************************")

def print_fila(df, enc):
	print("Ingrese el id de la fila: ")
	f = int(input())
	for e in enc:
		print("{}\t{}".format(e, df[e][f]))

def print_filas(df, enc):
	print("Ingrese el id de la fila N: ")
	f1 = int(input())
	print("Ingrese el id de la fila M: ")
	f2 = int(input())
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

if len(sys.argv) != 2:
  print("Error! en la cantidad de argumentos.")
  print(sys.argv[0], "<sensordatafile.csv>")
  sys.exit()



df_encabezados = ["id", "fecha", "tipo_elem", "intensidad", "ocupacion", "carga", "vmed", "error", "periodo_integracion"]
df = pd.read_csv(sys.argv[1], sep=';', quotechar='"')
# df = pd.read_csv('/home/carlos/ubuntu_transfer/temporal_series/sensores_madrid/11-2023.csv', sep=';', quotechar='"')

print("Archivo: ", sys.argv[1])
df_count = str(df.count())

print(df.head())
print(df.info())
print(df_count)

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
		print("Mostrando fila N")
		print_filas(df, df_encabezados)

	else:
		print("Opción no disponible... intentar de nuevo.")
	print_menu()
	opc = int(input())
print("Fin")