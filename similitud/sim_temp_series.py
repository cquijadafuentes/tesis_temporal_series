import sys
import numpy as np


def calc_euclidean(actual, predic):
    return np.sqrt(np.sum((actual - predic) ** 2))

def calc_mape(actual, predic):
    return np.mean(np.abs((actual - predic) / actual))

if len(sys.argv) != 2:
  print("Error! en la cantidad de argumentos.")
  print(sys.argv[0], "<filename.txt>")
  sys.exit()


rows = 0
cols = 0
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

print(lineas)
for i in lineas:
  npI = np.fromfile(i, dtype='int32')
  for j in lineas:
    npJ = np.fromfile(j, dtype='int32')
    ed = calc_euclidean(npI, npJ)
    print(f"{ed:.2f}", end=' ')
