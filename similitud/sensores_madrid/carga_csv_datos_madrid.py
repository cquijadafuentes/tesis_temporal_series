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