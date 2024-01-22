#include "../include/directorio.h"

void leer_nombres(char* path_carpeta){
    //Construyo el comando para listar los archivos de la carpeta y guardarlos en un txt
    size_t len = strlen("cd ") + strlen(path_carpeta) + strlen(" && ls > ../../") + strlen(ARCHIVOS_TEMP) + 1;
    char *comando = safe_malloc(len);
    snprintf(comando, len, "cd %s && ls > ../../%s", path_carpeta, ARCHIVOS_TEMP);
    safe_system(comando);
    free(comando);
}

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

    fclose(archivosTxt);

    // rm ARCHIVOS_TEMP
    size_t len = strlen("rm ") + strlen(ARCHIVOS_TEMP) + 1;
    char *comando = safe_malloc(len);
    snprintf(comando, len, "rm %s", ARCHIVOS_TEMP);
    safe_system(comando);
    free(comando);
    return directorio;
}

void free_dir(dir directorio)
{
    for (int i = 0; i < directorio.cantidad; i++)
    {
        safe_fclose(directorio.archivos[i]);
    }
    free(directorio.archivos);
    for (int i = 0; i < directorio.cantidad; i++)
    {
        free(directorio.nombres[i]);
    }
    free(directorio.nombres);
}