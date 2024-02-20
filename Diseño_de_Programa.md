# Trabajo Práctico Final - Programación II - UNR

Estudiante: Agustín Tomás Blando
Profesor: Federico Severino Guimpel

## Introducción

Este programa es una implementación de un sistema de predicción de texto. El objetivo es tomar un conjunto de archivos de texto, normalizar su contenido y luego utilizar un modelo de lenguaje para predecir la siguiente palabra en una secuencia de texto.

## Estructura 

El programa está estructurado en varios archivos de código C y Python, cada uno con una responsabilidad específica. 

La estructura del proyecto es la siguiente
```
.
├── bin
├── Entradas
├── Frases
├── include
│   ├── directorio.h
│   ├── normalizado_texto.h
│   ├── safe_functions.h
│   └── test_normalizado_texto.h
├── obj
├── src
│   ├── directorio.c
│   ├── main.c
│   ├── normalizado_texto.c
│   ├── output
│   ├── prediccion_texto.py
│   ├── safe_functions.c
│   └── test_normalizado_texto.c
├── test
└── Makefile

```

Esta es una estructura típica de un programa de C donde el directorio `obj` almacena los archivos de objectos `.o` requeridos para compilar y en `include` guardo los headers de los archivos de C.
En `bin` encontraremos el ejecutable del programa con el nombre `start`.
Los ejecutables de los test se guardan en `test`.
## C: Normalizado del Texto

### safe_functions
Para poder facilitar el manejo de errores en tiempo de ejecución, que puedan darse (por ejemplo, que el proceso no tenga permiso para leer el directorio textos) cree el archivo safe\_functions.
Las funciones declaradas en _safe_functions.h_ son las siguientes:

```c
/**
 * Asigna memoria del tamaño dado y verifica errores.
 * Sale del programa si la asignación falla.
 */
void *safe_malloc(size_t size);

/**
 * Reasigna memoria y verifica errores.
 * Sale del programa si la reasignación falla.
 */
void *safe_realloc(void *ptr, size_t size);

/**
 * Abre un archivo y verifica errores.
 * Sale del programa si la apertura falla.
 */
FILE *safe_fopen(const char *path, char *mode);

/**
 * Cierra un archivo y verifica errores.
 * Sale del programa si el cierre falla.
 */
int safe_fclose(FILE *fp);

/**
 * Ejecuta un comando del sistema y verifica errores.
 * Sale del programa si la ejecución falla.
 */
int safe_system(char *comando);
```

Con malloc o realloc, podemos revisar si fue posible asignar la memoria revisando si el puntero es distinto de null:

```c
void *safe_malloc(size_t size)
{
    void *ptr = malloc(size);
    if (ptr == NULL)
    {
        fprintf(stderr, "Malloc error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return ptr;
}
```

Con `safe_fclose`, si la función no devuelve 0 imprimimos un error a `stderr` y cerramos el programa devolviendo `EXIT_FAILURE` que es una constante igual a 1.

Con la función `system()` de C pasa algo particular. La función devuelve el valor de retorno del comando si no hubo ningún problema y -1  si hubo algún error. 
En caso de que haya algún problema con la ejecución de `system` , cuando intentaba imprimir el error usando `strerror(errno)` el output era:
```zsh
patob   diseno-programa-doc  bin/start hello_world
ERROR: Success
```
Además del error,  el mismo comando imprime a `stderr` cuando falla. Por eso no uso  `sterror` en `safe_system`
```c
int safe_system(char *comando)
{
    int sys_err = system(comando);
    if (sys_err)
    {
        fprintf(stderr, "Error al ejecutar el comando %s\n", comando); 
        //Intente mostrar el error que devolvia el comando cuando fallaba con strerror(errno) pero mostraba "success"
        exit(EXIT_FAILURE);
    }
    return sys_err;
}
```

### main.c
La ejecución del programa comienza con este archivo, aquí es donde se guarda el argumento pasado por consola, se llaman a todas las funciones principales de normalizado de texto y, una vez terminada la fase de normalización, se llama a la función _"run_python"_ que comienza la ejecución de la parte de Python.
```c
int main(int argc, char *argv[])
{
    if (argc > 2)
    {   
        fprintf(stderr, "ERROR: Demasiados argumentos.\n");
        exit(EXIT_FAILURE);
    }
    else if (argc < 2)
    {
        fprintf(stderr, "ERROR: Se espera al menos un argumento.\n");
        exit(EXIT_FAILURE);
    }
    
    char *path = path_textos(argv[1]);
    dir directorio = leer_directorio(path);

    char* out_file_path = safe_malloc(strlen(OUT_PATH) + strlen(argv[1]) + strlen(".txt") + 1);
    snprintf(out_file_path, strlen(OUT_PATH) + strlen(argv[1]) + strlen(".txt") + 1, "%s%s%s", OUT_PATH, argv[1], ".txt");
    write_directorio_normalizado(directorio, out_file_path);
    free(out_file_path);
    
    run_python(argv[1]);

    free(path);
    free_dir(directorio);

    return EXIT_SUCCESS;
}
```
### Leer directorio
El primer paso para leer un directorio es recopilar la información de los archivos dentro del mismo. Para esto utilizamos la función `system()` con el comando `cd Texto/arg && ls > ../../archivos.txt`. Esto nos guarda todos los archivos dentro del directorio con el nombre pasado como argumento dentro de `archivos.txt`.
#### struct dir
Para guardar la información del directorio, utilicé la siguiente estructura declarada en `directorio.h`

```c
typedef struct _dir
{
    FILE **archivos;
    char **nombres;
    int cantidad;
} dir;
```
* `archivos`: Es un array de archivos.
* `nombres`: Es un array de strings que contiene los nombres de los archivos (usado para debuggear el código)
* cantidad: Es un entero que representa la cantidad de archivos guardados en el directorio actual.

Los archivos y los nombres comparten el mismo index. O sea, que el puntero a FILE que le corresponde al archivo cuyo nombre se guarda en `nombres[0]` es `archivos[0]`. Esta manera de guardar los datos me permitió iterar sobre los directorios de manera muy sencilla.  
#### leer_directorio
La principal función que se encarga de leer los nombres de un directorio es la siguiente:
```c
dir leer_directorio(char *path_carpeta)
{
    leer_nombres(path_carpeta);

    FILE *archivosTxt = safe_fopen(ARCHIVOS_TEMP, "r"); //Archivos.txt es un archivo temporal que contiene los nombres de los archivos de la carpeta

    char **nombres = malloc(sizeof(*nombres));
    FILE **archivos = malloc(sizeof(*archivos));
    int index = -1;
    size_t line_len = 0;
    char* line = NULL;

    while((getline(&line, &line_len, archivosTxt)) != -1) 
    {
        index++;
        char *archivo = safe_malloc(strlen(line) + 1);
        strcpy(archivo, line);
        char *path_archivo;

        nombres = safe_realloc(nombres, sizeof(*nombres) * (index + 1));
        size_t len = strlen(archivo);
        archivo[len-- - 1] = '\0'; //Saco el salto de linea y resto 1 a len
        nombres[index] = safe_malloc(len + 1);
        strcpy(nombres[index], archivo);

        len = strlen(path_carpeta) + strlen(archivo) + strlen("/") + 1;
        path_archivo = safe_malloc(len);
        *path_archivo = '\0';
        snprintf(path_archivo, len, "%s/%s", path_carpeta, archivo);

        archivos = safe_realloc(archivos, sizeof(*archivos) * (index + 1));
        archivos[index] = safe_fopen(path_archivo, "r");

        free(path_archivo);
        free(archivo);
    }
    free(line);

    dir directorio = {archivos, nombres, index + 1};
    rm_temp();
    fclose(archivosTxt);

    return directorio;
}
```
Para poder trabajar con memoria dinámica, hago uso de la función `getline()`. Esta función toma como parámetro dos direcciones de memoria y un puntero FILE, el primero para guardar la línea, el segundo para guardar la longitud de esa línea, y el último es el stream o archivo del que se tomaran las líneas. 

La función se encargará de reallocar (o allocar si `line` apunta a null) la memoria necesaria para guardar las lineas del archivo a medida que lo vamos leyendo, por eso la variable `line` usa **memoria dinámica** y tengo que liberarla una vez termine de usarla. 


## Python: Predicción de texto

### Modelo

Para la implementación de la predicción de texto en python, decidí usar un modelo común mente usado en modelos de lenguaje natural: un bigrama.
Así es como creo un bigrama en python:

```python
def generar_ngrama(oracion, n):
    return [tuple(oracion[i:i+n]) for i in range(len(oracion)-n+1)]
```

La función sirve para generar un n-grama, pero en nuestro caso solo necesito generar bigramas.
Decidí usar bigramas porque es una manera simple de generar predicciones bastante acertadas. Además, por lo limitado de los datos de entrenamiento, no habría mejoras considerables de usar un n-grama de magnitud mayor.
## Entrenamiento del modelo
Si bien uso un bigrama, nuestro caso tenemos más contexto que solo la palabra precedente para adivinar la que sigue. Por eso guardo bi-gramas de palabras que normalmente se preceden y de palabras que normalmente van después de otras. 

```python
def entrenar_modelo(texto):
    modelo = defaultdict(lambda: defaultdict(lambda: defaultdict(int)))
    for oracion in texto:
        for w1, w2 in generar_ngrama(   oracion, 2):
            modelo[w1][SIG_PALABRA][w2] += 1
            modelo[w2][PREV_PALABRA][w1] += 1
    return modelo
```

```json
"ejemplo": {
            "sig": {
                "de": 1
            }
        },
        "de": {
            "prev": {
                "ejemplo": 1
            },
            "sig": {
                "archivo": 1
            }
        },
        "archivo": {
            "prev": {
                "de": 1
            },
            "sig": {
                "con": 1
            }
        },
        "con": {
            "prev": {
                "archivo": 1
            },
            "sig": {
                "espacios": 1
            }
        },
    (...)
```

### Predicción
Para predecir la palabra faltante de una oración oración, tomo la palabra que esté al lado del guión, la dirección en la que está, y busco en mi diccionario adecuado la palabra con mayor apariciones (la más probable).

```python
def get_mas_probable(palabra, direccion, modelo):
    if modelo[palabra]:
        return max(modelo[palabra][direccion].items(), key=lambda item: item[1])
    return None, 0
   
```

### Pesos

Mi primeras predicciones dan la misma prioridad a las palabras anteriores al guión como a las siguientes, como se ve en la siguiente linea de código:

```python
def predecir(oracion, modelo,):
    idx = oracion.index("_")
    palabra_sig_apariciones = 0
    palabra_prev_apariciones = 0

    # Hay palabra previa
    if idx > 0: 
        prev_word = oracion[idx-1]
        palabra_predict_prev, palabra_prev_apariciones = get_mas_probable(prev_word, SIG_PALABRA, modelo)

    # Hay palabra siguiente
    if idx < len(oracion) - 1: 
        sig_word = oracion[idx+1]
        palabra_predict_sig, palabra_sig_apariciones = get_mas_probable(sig_word, PREV_PALABRA, modelo)

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


