#include "../include/normalizado_texto.h"
#include <assert.h>
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
    assert(directorio.nombres != NULL);
    int bandera_archivo = 1;
    for (int i = 0; i < directorio.cantidad; i++) bandera_archivo = (strcmp(directorio.nombres[i], "!.txt") == 0); //Si el archivo !.txt esta en el directorio
    assert(bandera_archivo == 0);
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
    //Testeo de normalizar dir
    dir directorio = leer_directorio("Textos/hello_world");
    write_directorio_normalizado(directorio, "test.txt");
    FILE* test = safe_fopen("test.txt", "r");
    char* linea = NULL;
    size_t len = 0;
    int bandera_cadena = 1;
    while ((getline(&linea, &len, test)) != -1) bandera_cadena = (strcmp(linea, "buenos dias como estas esto es un lorem ipsum\n") == 0);
    assert(bandera_cadena == 0);
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