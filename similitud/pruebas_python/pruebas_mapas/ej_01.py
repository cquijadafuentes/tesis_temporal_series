import folium
import pandas as pd

# Leer los datos desde un archivo CSV
data = pd.read_csv('/home/carlos/ubuntu_transfer/temporal_series/sensores_madrid/pmed_ubicacion_10-2023.csv', sep=';', quotechar='"')

print(data.head())

# Crear un mapa centrado en Madrid
m = folium.Map(location=[40.4168, -3.7038], zoom_start=12)

# Agregar los marcadores
for index, row in data.iterrows():
    folium.Marker([row['latitud'], row['longitud']]).add_to(m)

# Mostrar el mapa
m