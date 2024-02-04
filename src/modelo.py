
texto = "Hola como estas\nHola como te llamas\nHola como estas amigo\nHola como estas bro\nHola como te va\n Estas bien amigo\nComo vas amigo\nComo viene tu dieta"

oracion1 = "Hola como _"
oracion2 = "como estas _"
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
} # El modelo es una representacion visual, la informacion no es precisa.

# El programa deberia recorrer la entrada una unica vez y generar el modelo.
# Para esto tengo que primero saber que oraciones quiero predecir, para poder contar sus ocurrencias en el archivo de entrada.

# Problema con el modelo: no siempre va a haber una oracion que siga la esttructura de la oracion a predecir.
# Tengo que dividir la oracion a predecir en un suboraciones y extender mi modelo.

modelo_oracion1 = {
    ["hola", "como", "_"] : {
        "estas" : 0.6, "te" : 0.4
        },
    ["como", "_"] : {
        "estas" : 0.6, "te" : 0.4
        }
}  
modelo_oracion2 = {
    ["como", "estas", "_"] : {
        "amigo" : 0.5, "bro" : 0.5
        },
    ["estas", "_"] : {
        "amigo" : 0.33, "bro" : 0.33, "bien" : 0.33
        }
}

# En realidad, hay informacion que estaria guardando que seria irrelenvante en este modelo.
# Si encuentro cadenas que coincidad con la estructura de mi oracion a predecir, no es necesario
# buscar pedazos mas pequeños de mi oracion a precedir, ya que los que encontre antes son mas especificos.

# Entonces, un modelo mas eficiente seria:
texto = "Hola como estas\nHola como te llamas\nHola como estas amigo\nHola como estas bro\nHola como te va\n Estas bien amigo\nComo vas amigo\nComo viene tu dieta"

modelo_eficiente_oracion1 = {
    ["hola", "como", "_"] : {
        "estas" : 0.6, "te" : 0.4
        }
}
modelo_eficiente_oracion2 = {
        ["como", "estas", "_"] : {
        "amigo" : 0.5, "bro" : 0.5
        }
}
modelo_eficiente_oracion3 = {   
    ["como", "_"] : {
        "estas" : 3/7, "te" : 2/7, "vas" : 1/7, "viene" : 1/7
    }
}

# Que pasaria en caso de que la palabra a predecir estuviera en el medio de la oracion y no
# encontrase ninguna cadena que coincidiera con mi oracion a predecir?
# Habria que tomar una decision. Ya dije que tomo la subcadena mas larga que coincida con mi oracion,
# pero, si dos subcadenas igual de largas coinciden con mi oracion, cual elijo?
# Una opcion es las dos! Tomo ambas subcadenas, hago los calculos de la palabra mas probable para cada una y elijo
# la de mayor magnitud.

oracion = "hola amigo _ estas bien\n"
texto = "hola amigo mio\ncomo estas bien\nseguro estas bien\nhola amigo queres ir a comer\nhola amigo queres ir a tomar algo\n"

modelo = {
    ["hola", "amigo", "_"] : {
        "mio" : 0.33, "queres" : 0.77
    },
    ["_" , "estas", "bien"] : {
        "como" : 0.5, "seguro" : 0.5
    }
}

# Tambien se me ocurre otro caso, en el que exita una cadena del estilo "amigo _ estas".
# Este caso tiene prioridad al anterior y se veria algo asi

oracion = "hola amigo _ estas bien\n"
texto = [["amigo", "como", "estas"], ["amigo", "seguro", "estas"], ["amigo", "como", "estas"], ["amigo", "mal", "estas"]] # Se me ocurrio guardar las oarciones asi
modelo = {
    ["amigo", "_", "estas"] : {
        "como" : 0.5, "seguro" : 0.25, "mal" : 0.25
    }
}