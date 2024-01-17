#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>
#include <sys/types.h>

#define ARCHIVOS_TEMP "archivos.txt"
#define OUT_PATH "Entradas/"
#define PYTHON_PROGRAM "prediccion_texto.py"

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

    char line[256];
    char **nombres = malloc(sizeof(*nombres));
    FILE **archivos = malloc(sizeof(*archivos));
    int index = -1;
    //TODO: Ver si puedo usar getline en vez de fgets.
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

char normalizar_char(char c)
{
    if islower(c) return c;
    else if (isupper(c)) return tolower(c);
    else switch(c){
        case '\n' : return ' ';
        case '.' : return '\n';
        case ' ' : return ' ';
        default : return '\0';
    }
}

char* normalizar_str(char* texto)
{
    char* texto_normalizado = safe_malloc(strlen(texto) + 1);
    int idx = 0;

    char norm_char;
    // Recorro el texto y los guardo en texto_normalizado
    while (*texto) if ((norm_char = normalizar_char(*texto++))) texto_normalizado[idx++] = norm_char; 
    texto_normalizado[idx] = '\0';
    texto_normalizado = safe_realloc(texto_normalizado, idx + 1);
    //Saco los espacios despues del punto
    if (*texto_normalizado == ' ') {
        char* new_normal_text = safe_malloc(strlen(texto_normalizado));
        *new_normal_text = '\0';
        strcpy(new_normal_text, texto_normalizado + 1);
        return new_normal_text;
    }
    return texto_normalizado;
}

int write_archivo_normalizado(FILE* archivo, FILE* salida)
{
    char* texto = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getdelim(&texto, &len, '.', archivo)) != -1) 
    {
        char* linea_normalizada = normalizar_str(texto);
        fputs(linea_normalizada, salida);
        free(linea_normalizada);
    }
    free(texto);
    return 0;
}

void normalizar_dir(dir directorio, char* path_salida)
{   
    FILE* salida = safe_fopen(path_salida, "w");

    for (int i = 0; i < directorio.cantidad; i++) write_archivo_normalizado(directorio.archivos[i], salida);

    safe_fclose(salida);
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

    //Testeo de normalizar_str
    char* texto;
    assert(!strcmp(texto = normalizar_str("Buenos dIAs,, ¿como estas? Es5to e43s UN lo--rem i-p-s-u-m"), "buenos dias como estas esto es un lorem ipsum"));
    free(texto);
    assert(!strcmp(texto = normalizar_str("/"), ""));
    free(texto);
    assert(!strcmp(texto = normalizar_str(" "), ""));
    free(texto);
    assert(!strcmp(texto = normalizar_str("HOLA AMIGO"), "hola amigo"));
    free(texto);
    assert(!strcmp(texto = normalizar_str("388-/hola"), "hola"));
    free(texto);

    
    printf("[ \xE2\x9C\x93 ] All tests passed.\n"); // check symbol TODO: Mover esto a los tests de python cuando esten hechos.
}

void run_python(char* arg)
{
    size_t len = strlen("python3 ") + strlen(PYTHON_PROGRAM) + strlen(" ") + strlen(arg) + 1;
    char *comando = safe_malloc(len);
    snprintf(comando, len, "python3 %s %s", PYTHON_PROGRAM, arg);
    safe_system(comando);
    free(comando);
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

    char* out_file_path = safe_malloc(strlen(OUT_PATH) + strlen(argv[1]) + strlen(".txt") + 1);
    snprintf(out_file_path, strlen(OUT_PATH) + strlen(argv[1]) + strlen(".txt") + 1, "%s%s%s", OUT_PATH, argv[1], ".txt");
    normalizar_dir(directorio, out_file_path);
    free(out_file_path);
    
    free(path);
    free_dir(directorio);

    run_python(argv[1]);
    return EXIT_SUCCESS;
}
