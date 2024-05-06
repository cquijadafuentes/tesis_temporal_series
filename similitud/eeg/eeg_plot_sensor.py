import sys
import numpy
import scipy.io as sio
import matplotlib.pyplot as plt

if len(sys.argv) != 2:
  print("Error! en la cantidad de argumentos.")
  print(sys.argv[0], "<filename.mat>")
  sys.exit()

mat = sio.loadmat(sys.argv[1])
data = mat['EEG'][0]['data'][0]

fig, ax = plt.subplots()
plt.plot(data[60], color='black')
plt.title("Prueba_60")
ax.set_ylabel("Señal")
ax.set_xlabel('Tiempo')
ax.grid(True)

fig.set_size_inches(15, 10)

plt.tight_layout()
plt.savefig("Prueba_60")