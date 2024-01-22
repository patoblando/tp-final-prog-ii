#include "../include/normalizado_texto.h"

char normalizar_char(char c)
{
    if (islower(c)) return c;
    else if (isupper(c)) return tolower(c); //Uso isupper, islower y tolower para evitar problemas con otros encodings
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
        free(texto_normalizado);
        return new_normal_text;
    }
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
