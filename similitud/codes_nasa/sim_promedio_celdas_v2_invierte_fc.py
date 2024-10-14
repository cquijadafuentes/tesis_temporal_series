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
proms = np.zeros((cols, rows))
print(len(lineas))
min = 1000
max = 0
for i in lineas:
  npI = np.fromfile(i, dtype='int32')
  index = 0;
  for c in range(cols):
    for f in range(rows):
      proms[cols-c-1][f] += int(npI[index])
      if(int(npI[index]) > max):
        max = int(npI[index])
      if(int(npI[index]) < min):
        min = int(npI[index])
      index += 1

print("# min =",min,"- max =",max)
min,max = max,min

for f in range(rows):
    for c in range(cols):
      proms[c][f] = float(int(proms[c][f]) / int(len(lineas)))
      print(proms[c][f], end='\t')
      if(proms[c][f] > max):
        max = proms[c][f]
      if(proms[c][f] < min):
        min = proms[c][f]
    print(" ")

print("# pMin =",min,"- pMax =",max)

# Configurar paleta de colores
cmap = sns.color_palette("coolwarm", as_cmap=True)

# Crear un mapa de calor
plt.figure(figsize=(8, 4))
plt.axis('off')
heatmap = sns.heatmap(proms, cmap=cmap, vmin=min, vmax=max)
plt.savefig(sys.argv[1] + '.heatmap.promedio_v2.png', bbox_inches='tight')
#plt.show()
plt.close()

