import sys
import numpy as np

if len(sys.argv) != 2:
    print("Error! en la cantidad de argumentos.")
    print(sys.argv[0], "<filename.txt>")
    sys.exit()

with open(sys.argv[1], 'r') as archivo:
    linea = archivo.readline()
    valores = [int(valor) for valor in linea.split()]
    n_rows, n_cols = valores[0], valores[1]
    grilla = []
    for linea in archivo:
        valores = [int(valor) for valor in linea.split()]
        grilla.append(valores)
    npGrila = np.fromarray
print(n_rows, n_cols)
print(grilla)

