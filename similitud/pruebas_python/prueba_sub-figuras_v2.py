import matplotlib.pyplot as plt
import numpy as np

# Generar datos de ejemplo
x = np.linspace(0, 10, 100)
y1 = np.sin(x)
y2 = np.cos(x)
y3 = np.tan(x)
y4 = np.exp(-x)

# Crear una figura con 4 subfiguras
fig, axs = plt.subplots(4, figsize=(10, 8))

# Subfigura 1
axs[0].plot(x, y1, 'r')
axs[0].set_title('Seno')

# Subfigura 2
axs[1].plot(x, y2, 'g')
axs[1].set_title('Coseno')

# Subfigura 3
axs[2].plot(x, y3, 'b')
axs[2].set_title('Tangente')

# Subfigura 4
axs[3].plot(x, y4, 'y')
axs[3].set_title('Exponencial Decay')

# Ajustar el layout para que no se solapen las subfiguras
plt.tight_layout()

# Mostrar la figura
plt.show()