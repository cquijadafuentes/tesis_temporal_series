import sys
import numpy as np
from esda.moran import Moran
from libpysal.weights import W, lat2W

if len(sys.argv) != 2:
  print("Error! en la cantidad de argumentos.")
  print(sys.argv[0], "<listfile.txt>")
  sys.exit()
print("DATASET: ", sys.argv[1])

lineas = []
with open(sys.argv[1], 'r') as file:
    firstLine = True
    for line in file:
        if(firstLine):
            nums = line.split()
            rows = int(nums[0])
            cols = int(nums[1])
            firstLine = False
        else:
            lineas.append(line.strip())

#print(lineas)
proms = np.zeros((rows, cols))
raster_type = np.dtype((np.int32, (rows, cols)))
w = lat2W(rows, cols, rook=False)
moran_i = []

for i in lineas:
    file_raster = open(i, "rb")
    npI = np.fromfile(file_raster, dtype=raster_type)[0]
    
    lm = Moran(npI, w, transformation='r', permutations=0)
    moran_i.append(lm.I)
    print(i, lm.I)