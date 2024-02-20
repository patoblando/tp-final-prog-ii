#ifndef NORMALIZADO_TEXTO_H
#define NORMALIZADO_TEXTO_H

#include <stdlib.h>
#include <stdio.h>
#include "../include/directorio.h"
#include <stdbool.h>


/**
 * Construye la ruta a los archivos de texto.
 */
char *path_textos(char *argumento);

/**
 * Normaliza un carácter de acuerdo a ciertas reglas.
 */
char normalizar_char(char c);

/**
 * Normaliza una cadena de acuerdo a ciertas reglas.
 */
char* normalizar_str(char* texto);

/**
 * Escribe un archivo normalizado a la salida.
 */
int write_archivo_normalizado(FILE* archivo, FILE* salida);

/**
 * Normaliza todos los archivos en un directorio.
 */
void write_directorio_normalizado(dir directorio, char* path_salida);

/**
 * Ejecuta un programa Python.
 */
void run_python(char* arg);

#endif // NORMALIZADO_TEXTO_H