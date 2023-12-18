import sys
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt

if len(sys.argv) != 2:
  print("Error! en la cantidad de argumentos.")
  print(sys.argv[0], "<filename.txt>")
  sys.exit()
print("DATASET: ", sys.argv[1])

lineas = []
with open(sys.argv[1], 'r') as file:
  firstLine = True
  for line in file:
    if(firstLine):
      nums = line.split()
      print(nums)
      rows = int(nums[0])
      cols = int(nums[1])
      firstLine = False
    else:
      lineas.append(line.strip())

#print(lineas)
print(len(lineas))
diffProm = np.zeros((rows, cols))
prevs = np.zeros((rows, cols))
firstLine = True
for i in lineas:
  npI = np.fromfile(i, dtype='int32')
  index = 0;
  if(firstLine):
    for c in range(cols):
      for f in range(rows):
        prevs[f][c] = int(npI[index])
        index += 1
    firstLine = False
  else:
    for c in range(cols):
      for f in range(rows):
        aux = abs(int(npI[index]) - prevs[f][c])
        diffProm[f][c] += aux
        prevs[f][c] = int(npI[index])
        index += 1

min = 100000
max = -min
divisor = (len(lineas)-1)
for f in range(rows):
    for c in range(cols):
      diffProm[f][c] = float(diffProm[f][c] / divisor)
      print(diffProm[f][c], end='\t')
      if(diffProm[f][c] > max):
        max = diffProm[f][c]
      if(diffProm[f][c] < min):
        min = diffProm[f][c]
    print(" ")
print("# diffPromMin =",min,"- diffPromMax =",max)

# Configurar paleta de colores
cmap = sns.color_palette("coolwarm", as_cmap=True)

# Crear un mapa de calor
heatmap = sns.heatmap(diffProm, cmap=cmap, cbar_kws={'label':'Promedio de Diferencias'}, vmin=min, vmax=max)
plt.savefig(sys.argv[1] + '.heatmap.diff_promedio.png', bbox_inches='tight')
#plt.show()

