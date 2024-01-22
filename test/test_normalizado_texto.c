#include "../include/normalizado_texto.h"
#include <stdio.h>
#include <stdlib.h>

void test_path_textos() 
{
    //Testeo de path_textos
    char *path = path_textos("hello_world");
    assert(strcmp(path, "Textos/hello_world") == 0);
    free(path);
}

void test_leer_directorio()
{
    //Testeo de leer_directorio
    dir directorio = leer_directorio("Textos/hello_world");
    assert(sizeof(directorio) == sizeof(dir));
    assert(directorio.archivos != NULL);
    assert(directorio.cantidad == 4);
    assert(strcmp(directorio.nombres[0], "!.txt") == 0);
    assert(directorio.archivos[0] != NULL);
    free_dir(directorio);
}
void test_normalizar_str()
{
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
}
void test_normalizar_dir()
{
    dir directorio = leer_directorio("Textos/hello_world");
    normalizar_dir(directorio, "test.txt");
    FILE* test = safe_fopen("test.txt", "r");
    char* linea = NULL;
    size_t len = 0;
    assert((getline(&linea, &len, test)) != -1);
    assert(!strcmp(linea, "hola como estas esto es un lorem ipsum\n"));
    safe_fclose(test);
    free(linea);
    free_dir(directorio);
    safe_system("rm test.txt");
    
}

int main(){
    test_path_textos();
    test_leer_directorio();
    test_normalizar_str();
    test_normalizar_dir();
    printf("[ \xE2\x9C\x93 ] All tests passed.\n"); // check symbol TODO: Mover esto a los tests de python cuando esten hechos.
    return 0;
}