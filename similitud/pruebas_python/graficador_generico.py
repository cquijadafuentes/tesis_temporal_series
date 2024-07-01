import sys
import matplotlib.pyplot as plt

print("Ingrese título de la gráfica:")
titulo = input().strip()
print("Ingrese cantidad de lineas a graficar:")
lineas = int(input().strip())
print("Ingrese cantidad de datos por cada linea:")
valores = int(input().strip())
print("Ingrese etiqueta para eje X:")
etiqX = input().strip()
print("Ingrese etiqueta para eje Y:")
etiqY = input().strip()
print("Ingrese los tics para el eje X:")
etiquetas = []
for i in range(valores):
	etiquetas.append(input().strip())

# Preparando entorno para gráficas:
fig, ax = plt.subplots()
minmax = []
for l in range(lineas):
	print("Ingrese los valores de la línea {} (separados por linea):".format(l+1))
	datos = []
	for v in range(valores):
		aux = float(input().strip())
		datos.append(aux)
	minmax.append(min(datos))
	minmax.append(max(datos))
	print("Ingrese la leyenda para esta linea:")
	etiqDatos = input().strip()
	plt.plot(etiquetas, datos, label=etiqDatos)

ax.set_yticks([min(minmax), max(minmax)])
plt.title(titulo)
ax.set_xlabel(etiqX)
ax.set_ylabel(etiqY)
ax.legend()
ax.grid(True)
fig.set_size_inches(12, 4)
plt.tight_layout()
plt.show()
plt.close()
