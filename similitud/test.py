import sys
import numpy as np

if len(sys.argv) != 2:
  print("Error! en la cantidad de argumentos.")
  print(sys.argv[0], "<filename.txt>")
  sys.exit()

npI = np.fromfile(sys.argv[1], dtype='int32')
for x in npI:
  print(x)