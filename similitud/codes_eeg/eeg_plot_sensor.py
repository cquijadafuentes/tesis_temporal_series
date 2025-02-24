import sys
import numpy
import scipy.io as sio
import matplotlib.pyplot as plt

if len(sys.argv) != 3:
  print("Error! en la cantidad de argumentos.")
  print(sys.argv[0], "<filename.mat> <sensor>")
  sys.exit()

mat = sio.loadmat(sys.argv[1])
ps = int(sys.argv[2])
data = mat['EEG'][0]['data'][0][ps]
et = str(mat['EEG'][0]['chanlocs'][0][0][ps][0][0])

fig, ax = plt.subplots()
plt.plot(data, color='black')
plt.title("Sensor " + sys.argv[2] + " [" + et + "]")
ax.set_ylabel("Señal")
ax.set_xlabel('Tiempo')
ax.grid(True)

fig.set_size_inches(12, 4)

plt.tight_layout()
#plt.savefig("Prueba_60")
plt.show()