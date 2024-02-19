#include "../include/safe_functions.h"

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

void *safe_realloc(void *ptr, size_t size)
{
    void *new_ptr = realloc(ptr, size);
    if (new_ptr == NULL)
    {
        fprintf(stderr, "Realloc error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return new_ptr;
}

FILE *safe_fopen(const char *path, char *mode)
{
    FILE *fp = fopen(path, mode);
    if (fp == NULL)
    {
        fprintf(stderr, "Error al abrir el archivo %s: %s\n", path, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return fp;
}
int safe_fclose(FILE *fp)
{
    int result = fclose(fp);
    if (result != 0)
    {
        fprintf(stderr, "Error al cerrar el archivo: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return result;
}


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