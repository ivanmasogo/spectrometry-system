import numpy as np
import pandas as pd
import joblib
import sys
from sklearn.preprocessing import StandardScaler
import json

# Función para cargar el modelo, escalador y PCA
def cargar_modelo():
    knn = joblib.load('modelo_knn.pkl')
    scaler = joblib.load('escalador.pkl')
    pca = joblib.load('pca.pkl')
    return knn, scaler, pca

# Función para cargar y preprocesar los datos del CSV
def preprocesar_datos(archivo_csv):
    # Lee el archivo CSV
    df = pd.read_csv(archivo_csv)
    
    # Lista de frecuencias a conservar
    frecuencias_deseadas = [410, 435, 460, 485, 510, 535]
    
    # Filtrar el DataFrame para mantener solo las filas con las frecuencias deseadas
    df_filtrado = df[df['Wavelength'].isin(frecuencias_deseadas)]
    
    # Verifica que el archivo CSV tenga las frecuencias esperadas
    if df_filtrado['Wavelength'].nunique() != len(frecuencias_deseadas):
        raise ValueError("El CSV no tiene todas las frecuencias esperadas.")
    
    # Convertir a formato adecuado para las predicciones
    df_transposed = df_filtrado.set_index('Wavelength').T  # Convertimos a formato largo (longitudes de onda como columnas)
    
    # Verifica que el archivo CSV tenga el formato adecuado (6 frecuencias)
    if df_transposed.shape[1] != 6:
        raise ValueError("El CSV no tiene el formato adecuado. Se esperaban 6 longitudes de onda.")
    
    return df_transposed

# Función para obtener las probabilidades de predicción para cada clase
def obtener_probabilidades(nuevas_muestras, modelo, scaler, pca):
    # Escalar los datos
    X_scaled = scaler.transform(nuevas_muestras)
    
    # Aplicar PCA para reducir a 3 componentes principales
    X_pca = pca.transform(X_scaled)
    
    # Obtener las probabilidades para cada clase
    probabilities = modelo.predict_proba(X_pca)
    
    return probabilities

# Función principal
def main():
    # Verificar que el archivo se pasa como argumento
    if len(sys.argv) != 2:
        print("Uso: python script.py <archivo_csv>")
        sys.exit(1)
    
    archivo_csv = sys.argv[1]
    
    try:
        # Cargar el modelo, el escalador y el PCA
        knn, scaler, pca = cargar_modelo()
        
        # Cargar y preprocesar los datos del archivo CSV
        nuevas_muestras = preprocesar_datos(archivo_csv)
        
        # Obtener las probabilidades de las predicciones
        probabilities = obtener_probabilidades(nuevas_muestras, knn, scaler, pca)
        
        # Mapeo de los índices numéricos a los nombres de los combustibles
        combustibles_map = {
            0: 'Diesel A', 
            1: 'Diesel A+', 
            2: 'Diesel B', 
            3: 'Diesel C', 
            4: 'Gasolina 95',
            5: 'Gasolina 98'
        }
        
        # Crear un diccionario con los resultados de las probabilidades
        resultados = {}
        for idx, prob in enumerate(probabilities[0]):
            combustible_name = combustibles_map[idx]  # Obtener el nombre del color usando el índice
            resultados[combustible_name] = prob * 100  # Guardar la probabilidad en porcentaje

        # Imprimir las probabilidades en formato JSON
        print(json.dumps(resultados))
        
    except Exception as e:
        print(f"Error: {e}")

# Ejecutar el script
if __name__ == "__main__":
    main()
