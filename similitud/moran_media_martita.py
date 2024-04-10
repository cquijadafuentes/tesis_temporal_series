#Given a raster time serie, compute the Moran's I for each raster

import sys
import numpy as np
from esda.moran import Moran
from libpysal.weights import W, lat2W
import matplotlib.pyplot as plt

if len(sys.argv) != 4:
	print("ERROR! USE " + sys.argv[0] + " <inputs_filename> <inputs_path_folder> <graphic_filename> ")
	sys.exit()

inputs_filename = sys.argv[1]
inputs_path_folder = sys.argv[2]
graphic_filename = sys.argv[3]

f = open(inputs_filename, "r")
line1 = f.readline().split()
n_rows = int(line1[0])
n_cols = int(line1[1])

raster_type = np.dtype((np.int32, (n_rows, n_cols)))
w = lat2W(n_rows, n_cols, rook=False)

moran_i = []

line1 = f.readline()
while line1:
	raster_filename = inputs_path_folder + "/" + line1
	raster_filename = raster_filename[:-1]
	file_raster = open(raster_filename, "rb")
	
	raster = np.fromfile(file_raster, dtype=raster_type)[0]
	file_raster.close()
	
	lm = Moran(raster, w, transformation='r', permutations=0)
	moran_i.append(lm.I)
	line1 = f.readline()
	
f.close()

fig, ax = plt.subplots()
plt.plot(moran_i, color='black')
plt.title(graphic_filename)
ax.set_ylabel("I' Moran")
ax.set_xlabel('Raster ID')
ax.grid(True)

fig.set_size_inches(15, 10)

plt.tight_layout()
plt.savefig(graphic_filename)

moran_i = np.array(moran_i)
moran_mean = np.mean(moran_i)
print('{0}\t{1}'.format(inputs_filename, moran_mean))