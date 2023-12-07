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

extern int errno;


int main(int argc, char *argv[]) {
    if( argc == 2 ) {
      printf("The argument supplied is %s\n", argv[1]);
   }
   else if( argc > 2 ) {
      printf("Too many arguments supplied.\n");
   }
   else {
      printf("ERROR: No se \n");
   }
    FILE *fp;
    fp = fopen(, "r");

    // Codigo aca

    printf("Hello World!\n");
    fclose(fp);
    return EXIT_SUCCESS;
}
