import sys
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt

def calc_euclidean(actual, predic):
    return np.sqrt(np.sum((actual - predic) ** 2))

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
mED = np.zeros((len(lineas), len(lineas)))
print(len(lineas))
f = 0;
c = 0;
for i in lineas:
  npI = np.fromfile(i, dtype='int32')
  for j in lineas:
    npJ = np.fromfile(j, dtype='int32')
    ed = calc_euclidean(npI, npJ)
    print(f"{ed:.2f}", end='\t')
    mED[f,c] = f"{ed:.2f}"
    c+=1
  f+=1
  c = 0

# Crear un mapa de calor
heatmap = sns.heatmap(mED, cmap="YlGnBu", annot=True)
#plt.show()

# Guardar la figura en un archivo (por ejemplo, en formato PNG)
imageName = sys.argv[1] + ".heatmap.png"
heatmap.get_figure().savefig(imageName, bbox_inches='tight')
