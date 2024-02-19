# Trabajo Práctico Final - Programación II - UNR

Estudiante: Agustín Tomás Blando
Profesor: Federico Severino Guimpel

## C: Normalizado del Texto

### Leer directorio

### Normalizar texto

### safe_functions

### main.c

## Python: Predicción de texto

### Modelo

## Entrenamiento del modelo
#### n-grama / bigrama

#### Modelo de lenguaje natural
#### Entrenamiento modelo

### Implementación


```python:n
def predecir(oracion, modelo,):
    idx = oracion.index("_")
    palabra_sig_apariciones = 0
    palabra_prev_apariciones = 0

    # Hay palabra previa
    if idx > 0: 
        prev_word = oracion[idx-1]
        palabra_predict_prev, palabra_prev_apariciones = predecir_palabra(prev_word, SIG_PALABRA, modelo)

    # Hay palabra siguiente
    if idx < len(oracion) - 1: 
        sig_word = oracion[idx+1]
        palabra_predict_sig, palabra_sig_apariciones = predecir_palabra(sig_word, PREV_PALABRA, modelo)

    # La palabra no esta en mi texto de entrenamiento
    if palabra_prev_apariciones == 0 and palabra_sig_apariciones == 0:
        palabra_random = random.choice(list(modelo.keys()))
        # Devuelvo una palabra al azar
        return palabra_random  if not DEBUGG else  "r_" + palabra_random 

    return palabra_predict_prev if palabra_prev_apariciones > palabra_sig_apariciones else palabra_predict_sig
   
```

### Pesos

Mi primeras predicciones dan la misma prioridad a las palabras anteriores al guión como a las siguientes, como se ve en la siguiente linea de código:

```python:n
def predecir(oracion, modelo,):
    idx = oracion.index("_")
    palabra_sig_apariciones = 0
    palabra_prev_apariciones = 0

    # Hay palabra previa
    if idx > 0: 
        prev_word = oracion[idx-1]
        palabra_predict_prev, palabra_prev_apariciones = predecir_palabra(prev_word, SIG_PALABRA, modelo)

    # Hay palabra siguiente
    if idx < len(oracion) - 1: 
        sig_word = oracion[idx+1]
        palabra_predict_sig, palabra_sig_apariciones = predecir_palabra(sig_word, PREV_PALABRA, modelo)

    # La palabra no esta en mi texto de entrenamiento
    if palabra_prev_apariciones == 0 and palabra_sig_apariciones == 0:
        palabra_random = random.choice(list(modelo.keys()))
        # Devuelvo una palabra al azar
        return palabra_random  if not DEBUGG else  "r_" + palabra_random 

    return palabra_predict_prev if palabra_prev_apariciones > palabra_sig_apariciones else palabra_predict_sig
   
```

Como se observa en la linea `return palabra_predict_prev if palabra_prev_apariciones > palabra_sig_apariciones else palabra_predict_sig` se da prácticamente la misma importancia a la cantidad de datos de apariciones de la palabra predicha utilizando la palabra anterior, que a la palabra predicha usando la palabra siguiente, excepto en caso de empate, que se escoge la predicha con la palabra anterior.

Esto me generó los el siguiente tipo de resultados
```zsh
patob   prediccion-texto -  make && bin/start Andres_Calamaro  
cc -I./include -g -Wextra -pedantic obj/safe_functions.o obj/direc  
torio.o obj/normalizado_texto.o obj/main.o -o bin/start  
cc -I./include -g -Wextra -pedantic obj/safe_functions.o obj/direc  
torio.o obj/normalizado_texto.o -o test/CTests src/test_normalizad  
o_texto.c    
['vencido', 'porque', 'el', 'cuerpo', '_de_', 'los', 'dos']  
['me', '_dejaste_', 'tus', 'punales', 'por', 'la', 'espalda']  
['el', 'placard', 'del', '_cuerpo_', 'de', 'juegos']  
['la', 'suerte', 'juega', 'con', 'cartas', '_r_la_', 'marcar']  
['te', 'olvidaste', 'ropa', 'en', '_el_', 'ropero']  
['no', 'se', 'si', '_estoy_', 'despierto']  
['dicen', 'que', 'hay', '_tendrias_', 'que', 'tener']  
['esta', '_en_', 'el', 'vecino', 'se', 'durmio']  
['y', '_te_', 'rendiste', 'en', 'febrero']  
['la', 'otra', '_y_', 'te', 'espere', 'bajo', 'la', 'lluvia']  
['me', 'quedo', 'con', 'lo', '_tendrias_', 'que', 'queda']  
['que', 'aprender', 'a', 'pedir', '_perdon_']  
['no', 'se', '_si_', 'estoy', 'dormido', 'o', 'sonando']  
['esta', 'vez', 'el', 'dolor', '_va_', 'a', 'terminar']  
['la', '_flor_', 'cayo', 'por', 'el', 'lado', 'de', 'la', 'soledad  
']  
['yo', 'camino', '_receta_', 'y', 'veo']  
['en', 'el', '_cuerpo_', 'de', 'la', 'tierra']  
['pero', 'te', '_quiero_', 'marzo']  
['no', 'perdi', '_del_', 'amor', 'al', 'canto']
```
_Nota: las palabra predichas por el algoritmo están entre guiones bajos_ _y las palabras que tienen _r fueron elegidas de manera aleatoria, ya que ni la palabra siguiente ni la anterior aparecen en el texto de entrenamiento.  

### Agregando pesos

Al agregar un sistema de pesos, por el cual se multiplican las apariciones de las palabras que preceden a la anterior o la siguiente, se consiguen un pequeño incremente de exactitud, o por lo menos de coherencia en las predicciones resultantes.
```python
def predecir(oracion, modelo, peso_ant = 0.66, peso_sig = 0.33):

	# (...)

    if palabra_prev_apariciones * peso_ant > palabra_sig_apariciones * peso_sig:
        return palabra_predict_prev
    else:
        return palabra_predict_sig
```

Los resultados de esta función son:

```zsh
 patob   prediccion-texto -  make && bin/start Andres_Calamaro
cc -I./include -g -Wextra -pedantic obj/safe_functions.o obj/directorio.o obj/normalizado_texto.o obj/main.o -o bin/start
cc -I./include -g -Wextra -pedantic obj/safe_functions.o obj/directorio.o obj/normalizado_texto.o -o test/CTests src/test_normalizado_texto.c 
['vencido', 'porque', 'el', 'cuerpo', '_de_', 'los', 'dos']
['me', '_dejaste_', 'tus', 'punales', 'por', 'la', 'espalda']
['el', 'placard', 'del', '_amor_', 'de', 'juegos']
['la', 'suerte', 'juega', 'con', 'cartas', '_r_florero_', 'marcar']
['te', 'olvidaste', 'ropa', 'en', '_el_', 'ropero']
['no', 'se', 'si', '_estoy_', 'despierto']
['dicen', 'que', 'hay', '_tendrias_', 'que', 'tener']
['esta', '_vez_', 'el', 'vecino', 'se', 'durmio']
['y', '_te_', 'rendiste', 'en', 'febrero']
['la', 'otra', '_y_', 'te', 'espere', 'bajo', 'la', 'lluvia']
['me', 'quedo', 'con', 'lo', '_tendrias_', 'que', 'queda']
['que', 'aprender', 'a', 'pedir', '_perdon_']
['no', 'se', '_si_', 'estoy', 'dormido', 'o', 'sonando']
['esta', 'vez', 'el', 'dolor', '_va_', 'a', 'terminar']
['la', '_flor_', 'cayo', 'por', 'el', 'lado', 'de', 'la', 'soledad']
['yo', 'camino', '_receta_', 'y', 'veo']
['en', 'el', '_mundo_', 'de', 'la', 'tierra']
['pero', 'te', '_quiero_', 'marzo']
['no', 'perdi', '_del_', 'amor', 'al', 'canto']
```

Donde apreciamos que la frase  _"esta _ el vecino se durmió"_ tiene mas sentido la predicción de la palabra _"vez"_ que la anterior predicción _"en"_.

En mi opinión darle más importancia a las palabras anteriores resulta generalmente en una predicción con mayo congruencia gramatical, debido a la naturaleza de nuestra escritura lineal.


