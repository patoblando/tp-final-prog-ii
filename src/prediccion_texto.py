text = "Hola como estas\nHola como te llamas\nHola como estas amigo\nHola como estas bro\nHola como te va"
text = text.split("\n")

oracion1 = "Hola como _"
oracion2 = "como _ estas"
oracion3 = "como _ te"

# Modelo de ejemplo de como almacenaria la informacion.
# Tengo un diccionario de dicionarios, donde la llave es una lista que representa la oracion a predecir
# y el valor es un diccionario donde la llave es las palabra que siguen a la oracion 
# en nuestro archivo de entrada y el valor es la cantidad de veces que aparece esa palabra dividido
# la cantidad de veces que aparece la oracion (con alguna palabra en el guion bajo) en el archivo de entrada.

oracion = ["hola", "como", "_"]
modelo = {
    ["hola", "como", "_"] : {
        "estas" : 0.5, "te" : 0.5
        },
    ["como", "estas", "_"] : {
        "amigo" : 0.5, "bro" : 0.5
        },
    ["como", "_", "te"] : {
        "llamas" : 0.5, "va" : 0.5
        }
}

# El programa deberia recorrer la entrada una unica vez y generar el modelo.
# Para esto tengo que primero saber que oraciones quiero predecir, para poder contar sus ocurrencias en el archivo de entrada.
