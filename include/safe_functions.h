#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

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
