/*
El programa en C tomar´a como argumento el nombre de la persona cuyos textos se van a analizar.
Dicho nombre no deber´a contener espacios, en caso de que el nombre contenga m´as de una palabra,
estas deber´an estar separadas por guiones bajos. Por ejemplo, si quisi´eramos analizar textos escritos por
Fito Paez, el argumento de entrada del programa deber´ıa ser Fito Paez. En el mismo directorio que el
programa se contar´a con una carpeta llamada Textos. Dentro de la misma podremos encontrar una serie
de carpetas, cada una de las cuales tendr´a el nombre de una persona respetando el mismo formato que
tiene el nombre recibido como argumento.
El programa deber´a acceder a la carpeta que corresponde al nombre recibido y procesar todos los archivos
que se encuentren dentro. Cada archivo contendr´a un texto que puede estar formado por muchas oraciones, puede contener saltos de l´ınea en cualquier lugar y tambi´en s´ımbolos de todo tipo (por ejemplo:
coma, punto y coma, gui´on, signos de pregunta, etc.). En los textos a analizar no habr´a palabras con
tilde ni letras especiales como la ~n. El objetivo es limpiar todos los textos y unificarlos en un ´unico
archivo de salida que deber´a tener las siguientes caracter´ısticas:
• El archivo deber´a tener el mismo nombre que la carpeta inspeccionada, es decir, el nombre recibido
como argumento. El mismo tendr´a la extensi´on .txt.
• El archivo generado deber´a ubicarse dentro de una carpeta llamada Entradas, la cual tambi´en se
encontrar´a en el mismo directorio que el programa actual.
• Dentro del archivo s´olo deber´a haber caracteres correspondientes a letras min´usculas y saltos de
l´ınea. Deber´an quitarse todos los s´ımbolos que se encuentren en los textos analizados y las letras
may´usculas deber´an pasarse a min´uscula.
• Cada l´ınea del archivo deber´a corresponder a una oraci´on de uno de los textos analizados. Se puede
identificar el fin de una oraci´on por la presencia del punto. Los saltos de l´ınea que se encuentren
en los textos que no est´en al final de una oraci´on deber´an quitarse tambi´en.

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

/* FILE * abrir_archivo(char *path)
{
    FILE *fp;
    fp = safe_fopen(path, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "ERROR: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return fp;
}

char * leer_archivo(FILE *fp)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        printf("Retrieved line of length %zu:\n", read);
        printf("%s", line);
    }
    return line;
} */

typedef struct _dir
{
    FILE **archivos;
    char **nombres;
    int cantidad;
} dir;

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

char *path_textos(char *argumento)
{
    const char *textos_path = "Textos/";
    char *path = safe_malloc(strlen(textos_path) + strlen(argumento) + 1);
    *path = '\0';
    strcat(path, textos_path);
    strcat(path, argumento);
    return path;
}

int safe_system(char *comando)
{
    int sys_err = system(comando);
    if (sys_err)
    {
        fprintf(stderr, "Error al ejecutar el comando %s\n", comando); //Intente mostrar el error que devolvia el comando con strerror(errno) pero mostraba "success"
        exit(EXIT_FAILURE);
    }
    return sys_err;
}

dir leer_directorio(char *path_carpeta)
{
    const char *archivos_path = "archivos.txt";
    
    //Construyo el comando para listar los archivos de la carpeta y guardarlos en un txt
    size_t len = strlen("cd ") + strlen(path_carpeta) + strlen(" && ls > ../../") + strlen(archivos_path) + 1;
    char *comando = safe_malloc(len);
    snprintf(comando, len, "cd %s && ls > ../../%s", path_carpeta, archivos_path);
    safe_system(comando);
    free(comando);

    FILE *archivosTxt = safe_fopen(archivos_path, "r"); //Archivos.txt es un archivo temporal que contiene los nombres de los archivos de la carpeta

    char line[256];
    char **nombres = malloc(sizeof(*nombres));
    FILE **archivos = malloc(sizeof(*archivos));
    int index = -1;

    while (fgets(line, sizeof(line), archivosTxt)) //Recorro el archivo temporal con los nombres de los archivos del directorio
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

    dir directorio = {archivos, nombres, index + 1};

    fclose(archivosTxt);
    safe_system("rm archivos.txt");
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

void tests() //TODO: Mover los test a un archivo aparte que se pueda ejecutar, en caso que no sea mucho bardo, si no, lo dejo asi.
{
    //Testeo de path_textos
    char *path = path_textos("hello_world");
    assert(strcmp(path, "Textos/hello_world") == 0);
    free(path);

    //Testeo de leer_directorio
    dir directorio = leer_directorio("Textos/hello_world");
    assert(sizeof(directorio) == sizeof(dir));
    assert(directorio.archivos != NULL);
    assert(directorio.cantidad == 4);
    assert(strcmp(directorio.nombres[0], "!.txt") == 0);
    assert(directorio.archivos[0] != NULL);

    free_dir(directorio);
    
    printf("[ \xE2\x9C\x93 ] All tests passed.\n"); // check symbol
}

char normalizar_char(char c)
{
    if (c >= 'a' && c <= 'z') return c;
    else if (c >= 'A' && c <= 'Z') return c + 32;
    else if (c == '\n') return ' ';
    else if (c == '.') return '\n'; //TODO: Cambiar esto en base si voy a pasarle a la funcion una unica linea o todo el contenido del archivo.
    else if (c == ' ') return ' ';
    else return '\0';
}

char* normalizar_str(char* texto)
{
    char* texto_normalizado = safe_malloc(strlen(texto) + 1);
    int idx = 0;

    char norm_char;
    while (*texto) if ((norm_char = normalizar_char(*texto++))) texto_normalizado[idx++] = norm_char; // Solo guardo los caracteres si cumplen con la condicion de normalizacion

    texto_normalizado[idx] = '\0';
    texto_normalizado = safe_realloc(texto_normalizado, idx + 1);
    return texto_normalizado;
}
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
    tests();
    
    char *path = path_textos(argv[1]);
    dir directorio = leer_directorio(path);

    for (int i = 0; i < directorio.cantidad; i++)
    {
        printf(" ---- %s ----\n", directorio.nombres[i]);
        char line[256];
        while (fgets(line, sizeof(line), directorio.archivos[i]))
        {
            printf("%s", line);
        }
        printf("\n");
    }

    char * texto_normalizado = normalizar_str("HOLA\nCOMO ESTAaS!!, me llamo RA-Ul y me Gusta [el] PENé. es útil para escriviR!!!!?\\?'??¡-__-uwu\n");
    printf("%s\n", texto_normalizado);
    free(texto_normalizado);
    
    free(path);
    free_dir(directorio);
    return EXIT_SUCCESS;
}
