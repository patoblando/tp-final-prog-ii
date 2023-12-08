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


/* FILE * abrir_archivo(char *path)
{
    FILE *fp;
    fp = fopen(path, "r");
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

char * path_textos(char *argumento)
{
    const char *textos_path = "Textos/";
    char *path = malloc(strlen(textos_path) + strlen(argumento) + 1);
    *path = '\0';
    strcat(path, textos_path);
    strcat(path, argumento);
    return path;
}

FILE ** leer_directorio(char *path_carpeta)
{
    const char * archivos_path = "archivos.txt";

    size_t len = strlen("cd ") + strlen(path_carpeta) + strlen(" && ls > ../../") + strlen(archivos_path) + 1;
    char * comando = malloc(len);
    snprintf(comando, len, "cd %s && ls > ../../%s", path_carpeta, archivos_path);
    system(comando);
    free(comando);
    
    FILE *fp = fopen(archivos_path, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "ERROR: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    char line[256];
    FILE * buf[256];
    int index = 0;
    while (fgets(line, sizeof(line), fp))
    {
        char *archivo = malloc(strlen(line) + 1);
        strcat(archivo, line);
        char * path_archivo;

        len = strlen(path_carpeta) + strlen(archivo) + 1;
        path_archivo = malloc(len); //FIXME: Fix the path to the archivo.

        printf("%s\n", path_archivo);
        buf[index] = fopen(path_archivo, "r");
        
        if (buf[index] == NULL)
        {
            fprintf(stderr, "ERROR No se pudo abrir el archivo %s: %s\n", path_archivo, strerror(errno));
            exit(EXIT_FAILURE);
        }
        
        index++;
    }
    FILE ** archivos = malloc(sizeof(FILE*) * index + 1);
    printf("index: %d\n", index);
    for(; buf[index]; index--){
        archivos[index] = buf[index];
        printf("archivo: %d\n", index);
    } 
    fclose(fp);
    return archivos;
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

    char *path = path_textos(argv[1]);
    FILE ** archivos = leer_directorio(path);
    for (int i = 0; archivos[i]; i++)
    {
        printf("archivo: %d\n", i);
        char line[256];
        while(fgets(line, sizeof(line), archivos[i]))
        {
            printf("test:");
            printf("%s", line);
        }
    }
    free(path);
    return EXIT_SUCCESS;
}
