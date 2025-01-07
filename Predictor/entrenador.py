import numpy as np
import pandas as pd
import os
from sklearn.decomposition import PCA
from sklearn.neighbors import KNeighborsClassifier
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import MinMaxScaler
import joblib
from sklearn.metrics import accuracy_score, confusion_matrix, classification_report
import matplotlib.pyplot as plt

# Función para cargar los archivos CSV de una subcarpeta y asignarles la etiqueta de combustible
def cargar_datos_csv(combustible, folder_path):
    data_frames = []
    for subfolder in os.listdir(folder_path):  # Recorrer subcarpetas dentro de 10ms y 20ms
        subfolder_path = os.path.join(folder_path, subfolder)
        if os.path.isdir(subfolder_path):  # Verificar que sea una subcarpeta
            for filename in os.listdir(subfolder_path):  # Recorrer los archivos CSV en cada subcarpeta
                if filename.lower().endswith('.csv'):
                    file_path = os.path.join(subfolder_path, filename)
                    # Leer el CSV
                    df = pd.read_csv(file_path)
                    
                    # Asegurarse de que cada archivo tenga las columnas correctas
                    if not df.empty and 'Wavelength' in df.columns and 'Value' in df.columns:

                        # Convertirlo a formato adecuado
                        df_transposed = df.set_index('Wavelength').T  # Convertir a formato largo (longitudes de onda como columnas)
                        df_transposed['Combustible'] = combustible  # Añadir la etiqueta del combustible
                        data_frames.append(df_transposed)
                    else:
                        print(f"El archivo {filename} no tiene las columnas esperadas o está vacío.")
    return data_frames

# Ruta a la carpeta principal que contiene las subcarpetas por combustible
carpeta_principal = 'Combustibles/'

# Combustibles y sus respectivas subcarpetas
combustibles = ['Diesel A', 'Diesel A+', 'Diesel B', 'Diesel C', 'Gasolina 95', 'Gasolina 98']

# Crear una lista para almacenar los DataFrames de todos los combustibles
data_frames = []

# Cargar los datos de cada combustible
for combustible in combustibles:
    for ms in ['10ms','20ms']:
        folder_path = os.path.join(carpeta_principal, combustible, ms)
        if os.path.exists(folder_path):
            combustible_data_frames = cargar_datos_csv(combustible, folder_path)
            data_frames.extend(combustible_data_frames)  # Añadir los datos de ese combustible
        else:
            print(f"La carpeta para {combustible} no se encuentra: {folder_path}")
        
# Concatenar los DataFrames de todos los combustibles en uno solo
df_completo = pd.concat(data_frames, ignore_index=True)

# Verificar que los datos se han cargado correctamente
print("Datos cargados:")
print(df_completo)  # Mostrar las primeras filas para verificar

# Separar las características (espectros) y las etiquetas (combustibles)
X = df_completo.drop('Combustible', axis=1).values  # Las características son todas las columnas excepto 'Combustible'
y = df_completo['Combustible'].values  # La etiqueta es la columna 'Combustible'

# Convertir las etiquetas de Combustible a valores numéricos
combustibles_map = {
    'Diesel A': 0,
    'Diesel A+': 1, 
    'Diesel B': 2, 
    'Diesel C': 3,
    'Gasolina 95': 4,
    'Gasolina 98': 5
}
y_numeric = np.array([combustibles_map[combustible] for combustible in y])

# Escalar los datos (esto es importante para PCA y KNN)
scaler = MinMaxScaler()
X_scaled = scaler.fit_transform(X)
print("\nMuestra normalizada:")
print(X_scaled)

# Aplicar PCA (Reducimos a 3 componentes principales)
pca = PCA(n_components=3)
X_pca = pca.fit_transform(X_scaled)

# Dividir en conjunto de entrenamiento y prueba
X_train, X_test, y_train, y_test = train_test_split(X_pca, y_numeric, test_size=0.2, random_state=42)

# Entrenar el modelo KNN
knn = KNeighborsClassifier(n_neighbors=5)
knn.fit(X_train, y_train)

# Realizar predicciones en el conjunto de prueba
y_pred = knn.predict(X_test)

# Calcular la precisión
accuracy = accuracy_score(y_test, y_pred)
print(f'Precisión del modelo: {accuracy * 100:.2f}%')

# Mostrar la matriz de confusión
conf_matrix = confusion_matrix(y_test, y_pred)
print("Matriz de Confusión:")
print(conf_matrix)

# Mostrar el informe de clasificación
class_report = classification_report(y_test, y_pred)
print("Informe de Clasificación:")
print(class_report)

# Obtener las probabilidades de cada clase para las muestras de prueba
probabilities = knn.predict_proba(X_test)

# Crear un DataFrame para las probabilidades
probabilities_df = pd.DataFrame(probabilities, columns=knn.classes_)

# Mostrar el DataFrame de probabilidades
print("Probabilidades de cada clase para las muestras de prueba:")
print(probabilities_df)

# Mostrar también las predicciones junto con las probabilidades
probabilities_df['Predicción'] = y_pred

# Mostrar las predicciones y las probabilidades
print("Predicciones con sus probabilidades:")
print(probabilities_df)

# Guardar el modelo, el escalador y PCA
joblib.dump(knn, 'modelo_knn.pkl')
joblib.dump(scaler, 'escalador.pkl')
joblib.dump(pca, 'pca.pkl')

print("Modelos guardados exitosamente.")

# --- Visualización de los datos en 2D ---

# Diccionario de combustibles para mapear las clases numéricas
combustiblemap = {
    0: 'Diesel A',
    1: 'Diesel A+',
    2: 'Diesel B', 
    3: 'Diesel C',
    4: 'Gasolina 95', 
    5: 'Gasolina 98'
}

# Diccionario de colores para mapear los combustibles
combustible_colores = {
    'Diesel A': 'blue',   # Combustible para 'diesel a'
    'Diesel A+': 'green',  # Combustible para 'diesel a+'
    'Diesel B': 'red',    # Combustible para 'diesel b'
    'Diesel C': 'yellow',  # Combustible para 'diesel c'
    'Gasolina 95': 'black',  # Combustible para 'gasolina 95'
    'Gasolina 98': 'pink' # Combustible para 'gasolina 98'

}

# Gráfico para el conjunto de entrenamiento
plt.figure(figsize=(12, 6))

# Primer gráfico: Conjunto de entrenamiento
plt.subplot(1, 2, 1)
for combustible_num, combustible in combustiblemap.items():
    mask = y_train == combustible_num  # Usamos los valores numéricos para crear la máscara
    plt.scatter(X_train[mask, 0], X_train[mask, 1], 
                c=combustible_colores[combustible],  # Usamos el diccionario para asignar combustibles
                label=combustible, marker='o')
plt.title('Conjunto de Entrenamiento')
plt.xlabel('Componente Principal 1')
plt.ylabel('Componente Principal 2')
plt.legend()

# Segundo gráfico: Conjunto de prueba
plt.subplot(1, 2, 2)
for combustible_num, combustible in combustiblemap.items():
    mask = y_pred == combustible_num  # Usamos los valores numéricos para crear la máscara
    plt.scatter(X_test[mask, 0], X_test[mask, 1], 
                c=combustible_colores[combustible],  # Usamos el diccionario para asignar combustibles
                label=combustible, marker='o')
plt.title('Conjunto de Prueba')
plt.xlabel('Componente Principal 1')
plt.ylabel('Componente Principal 2')
plt.legend()

# Mostrar ambos gráficos
plt.tight_layout()
plt.show()
