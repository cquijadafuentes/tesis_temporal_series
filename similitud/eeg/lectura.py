# https://stackoverflow.com/questions/874461/read-mat-files-in-python
# https://docs.scipy.org/doc/scipy/reference/generated/scipy.io.loadmat.html

import sys
import numpy
import scipy.io

if len(sys.argv) != 2:
  print("Error! en la cantidad de argumentos.")
  print(sys.argv[0], "<filename.mat>")
  sys.exit()

print("file:", sys.argv[1])

mat = scipy.io.loadmat(sys.argv[1])


# Print the keys of the MATLAB dictionary
print(mat.keys())

# Print the type of the value corresponding to the key 'EEG'
print(type(mat['EEG']))

# Print the shape of the value corresponding to the key 'EEG'
print(numpy.shape(mat['EEG']))

print(type(mat['EEG'][0]))
print(type(mat['EEG'][0][0]))
print(numpy.shape(mat['EEG'][0][0]))
print(numpy.size(mat['EEG'][0][0]))
