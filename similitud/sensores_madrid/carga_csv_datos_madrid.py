import sys
import pandas as pd

if len(sys.argv) != 2:
  print("Error! en la cantidad de argumentos.")
  print(sys.argv[0], "<sensordatafile.csv>")
  sys.exit()

df = pd.read_csv(sys.argv[1], sep=';', quotechar='"')

print(df.head())
print(df.iloc[:4])

prev = df['id'][0]
contador = 1;
for id in df['id']:
	if id != prev:
		contador += 1
		prev = id

print(contador)

promedioMuestras = 0.0
c = {}
for id in df['id']:
	promedioMuestras += 1
	if id not in c:
		c[id] = 1
	else:
		c[id] += 1

print("Histograma de muestras por sensor")
print("id", "muestras")
for k in c:
	print(k, c[k])

print()
promedioMuestras /= len(c)
print("Promedio de muestras por sensor: ", promedioMuestras)
print("Cantidad de sensores: ", len(c))

vmin = min(c.values())
vmax = max(c.values())
print("min: {} - max: {}".format(vmin, vmax))

print("Sensores con menos muestras:")
print("id", "muestras")
for k in c:
	if(c[k] < promedioMuestras):
		print(k, c[k])
