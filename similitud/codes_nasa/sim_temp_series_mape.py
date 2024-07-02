import sys
import numpy as np


def calc_mape(actual, predic):
    return np.mean(np.abs((actual - predic) / (actual + 0.000001)))

if len(sys.argv) != 2:
  print("Error! en la cantidad de argumentos.")
  print(sys.argv[0], "<filename.txt>")
  sys.exit()
print("MAPE DATASET: ", sys.argv[1])


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
mMAPE = np.zeros((len(lineas), len(lineas)))
print(mMAPE.shape)
f = 0;
c = 0;
for i in lineas:
  npI = np.fromfile(i, dtype='int32')
  for j in lineas:
    npJ = np.fromfile(j, dtype='int32')
    mape = calc_mape(npI, npJ)
    #print(f"{mape:.2f}", end=' ')
    print(mape)
    mMAPE[f,c] = f"{mape:.2f}"
    c+=1
  f+=1
  c = 0
print(mMAPE)

