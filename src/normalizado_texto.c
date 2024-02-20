#include "../include/normalizado_texto.h"

char *path_textos(char *argumento)
{
    const char *textos_path = "Textos/";
    char *path = safe_malloc(strlen(textos_path) + strlen(argumento) + 1);
    *path = '\0';
    strcat(path, textos_path);
    strcat(path, argumento);
    return path;
}

char normalizar_char(char c)
{
    if (islower(c)) return c;
    else if (isupper(c)) return tolower(c); //Uso isupper, islower y tolower para evitar problemas con otros encodings
    else switch(c){
        case '\n' : return ' ';
        case ' ' : return ' ';
        default : return '\0';
    }
}

char* normalizar_str(char* texto)
{
    char* texto_normalizado = safe_malloc(strlen(texto) + 1);
    int idx = 0;
    char* char_ptr = texto;
    char norm_char;
    // Recorro el texto, normalizando los caracteres y los guardo en texto_normalizado
    while (*char_ptr) {
        norm_char = normalizar_char(*char_ptr);
        bool isCurrentAndNextCharSpaceOrNewline = (*char_ptr == ' ' || *char_ptr == '\n') && (idx == 0 || *(char_ptr + 1) == ' ' || *(char_ptr + 1) == '\n')
        if (norm_char && !isCurrentAndNextCharSpaceOrNewline) {
            texto_normalizado[idx++] = norm_char;
        }
        char_ptr++;
    }
    texto_normalizado[idx] = '\0';
    texto_normalizado = safe_realloc(texto_normalizado, idx + 1); //Encojo la memoria allocada
    return texto_normalizado;
}

int write_archivo_normalizado(FILE* archivo, FILE* salida)
{
    char* texto = NULL;
    size_t len = 0;
    while ((getdelim(&texto, &len, '.', archivo)) != -1) 
    {
        char* linea_normalizada = normalizar_str(texto);
        fputs(linea_normalizada, salida);
        fputs("\n", salida);
        free(linea_normalizada);
    }
    free(texto);
    return 0;
}

void write_directorio_normalizado(dir directorio, char* path_salida)
{   
    FILE* salida = safe_fopen(path_salida, "w");

    for (int i = 0; i < directorio.cantidad; i++) write_archivo_normalizado(directorio.archivos[i], salida);

    safe_fclose(salida);
}
