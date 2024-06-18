import sys
import matplotlib.pyplot as plt

print("Ingrese cantidad de lineas a graficar:")
lineas = int(input().strip())
print("Ingrese cantidad de datos por cada linea:")
valores = int(input().strip())

# Preparando entorno para gráficas:
fig, ax = plt.subplots()

etiquetas = ["intensidad", "ocupacion", "carga", "vmed"]

for l in range(lineas):
	print("Ingrese los valores de la línea {} (separados por linea):".format(l+1))
	datos = []
	for v in range(valores):
		aux = float(input().strip())
		datos.append(aux)
	print("Ingrese la etiqueta para esta linea:")
	etiqDatos = input().strip()
	plt.plot(etiquetas, datos, label=etiqDatos)

ax.set_yticks([-1, -0.5, 0, 0.5, 1])
print("Ingrese título de la gráfica:")
titulo = input().strip()
plt.title(titulo)
print("Ingrese etiqueta para eje X:")
etiqX = input().strip()
ax.set_xlabel(etiqX)
print("Ingrese etiqueta para eje Y:")
etiqY = input().strip()
ax.set_ylabel(etiqY)
ax.legend()
ax.grid(True)
fig.set_size_inches(12, 4)
plt.tight_layout()
plt.show()
plt.close()
