#include "../include/normalizado_texto.h"

#define OUT_PATH "Entradas/"
#define PYTHON_PROGRAM "prediccion_texto.py"

void run_python(char* arg)
{
    size_t len = strlen("python3 ") + strlen("src/") + strlen(PYTHON_PROGRAM) + strlen(" ") + strlen(arg) + 1;
    char *comando = safe_malloc(len);
    snprintf(comando, len, "python3 src/%s %s", PYTHON_PROGRAM, arg);
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
    //tests();
    
    char *path = path_textos(argv[1]);
    dir directorio = leer_directorio(path);

    char* out_file_path = safe_malloc(strlen(OUT_PATH) + strlen(argv[1]) + strlen(".txt") + 1);
    snprintf(out_file_path, strlen(OUT_PATH) + strlen(argv[1]) + strlen(".txt") + 1, "%s%s%s", OUT_PATH, argv[1], ".txt");
    normalizar_dir(directorio, out_file_path);
    free(out_file_path);
    
    free(path);
    free_dir(directorio);

    //run_python(argv[1]);
    return EXIT_SUCCESS;
}