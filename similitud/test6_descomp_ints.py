import numpy as np
import statsmodels.api as sm
import matplotlib.pyplot as plt

# Generar datos de ejemplo (valores enteros)
np.random.seed(0)
datos_enteros = np.random.randint(0, 100, 15)

# Realizar la descomposición estacional
descomposicion = sm.tsa.seasonal_decompose(datos_enteros, model='additive', period=7)

# Graficar los componentes de la descomposición
plt.figure(figsize=(10, 6))

plt.subplot(411)
plt.plot(datos_enteros, label='Datos originales')
plt.legend()

plt.subplot(412)
plt.plot(descomposicion.trend, label='Tendencia')
plt.legend()

plt.subplot(413)
plt.plot(descomposicion.seasonal, label='Estacionalidad')
plt.legend()

plt.subplot(414)
plt.plot(descomposicion.resid, label='Residuos')
plt.legend()

plt.tight_layout()
plt.show()

print(datos_enteros)
print(descomposicion.trend)
print(descomposicion.seasonal)
print(descomposicion.resid)