#include <stdlib.h>
#include <stdio.h>
#include "../include/safe_functions.h"
#define ARCHIVOS_TEMP "archivos.txt"

typedef struct _dir
{
    FILE **archivos;
    char **nombres;
    int cantidad;
} dir;

/**
 * Lee un directorio y devuelve una estructura que contiene los archivos, sus nombres y la cantidad de archivos.
 */
dir leer_directorio(char *path_carpeta);

/**
 * Libera la memoria asignada para una estructura de directorio.
 */
void free_dir(dir directorio);

/**
 * Lee los nombres de los archivos en un directorio.
 */
void leer_nombres(char* path_carpeta);

/**
 * Borra el archivo temporal que contiene los nombres de los archivos.
 */
void rm_temp();