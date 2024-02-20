
# Trabajo Práctico Final - Programación II - Predicción de texto

Un programa simple de predicción de texto hecho en C y Python como trabajo final de la materia "Programación II" en la carrera de Licenciatura en Ciencias de la Computación de la UNR.

[[Diseño de Programa]]
## Prerequisitos
**El programa es compatible _solo_ con sistemas Unix
Para compatibilad en Windows, usar wsl (https://learn.microsoft.com/en-us/windows/wsl/install)**

- GCC (https://gcc.gnu.org/)
- Python3 (https://www.python.org)
- git (https://git-scm.com/)
- Make (https://www.gnu.org/software/make/)

## Clonar repositorio  

  Para clonar el repositorio, ejecute el siguiente comando:
  
  ```sh
  git clone https://github.com/patoblando/tp-final-prog-ii/
  ```
  
## Como compilar y ejecutar

  Para compilar, simplemente correr:

 ```sh
  make
  ```

  Para correr el programa, ejecute el programa de C recién creado, este correrá el código de python
 ```sh
  bin/start {{arg}} 
  ```
  
  > **Nota:** Se puede coompilar solo el codigo de C y n los test usando ```make start```

## Tests
  Para compilar los test en C, se puede correr:
  ```sh
  make test
  ```
  > **Nota:** Correr solo make también coompila los tests

  Y para correr los tests de C, ejecute:

  ```sh
  test/CTests
  ```
  Los tests de python usan pytest,  asegurate de instalarlo usando

  ```sh
  pip3 install pytest
  ```
  Y luego, puedes correr los test con:

  ```sh
  python3 -m pytest src/test_predecir.py
  ```

  Los tests del programa en C fallaran si el programa no puede encontrar el directorio Textos/hello_world   con sus archivos adentro.
  No modifiques estos archivos o el programa no funcionara correctamente.

  > **test:** Correr solo make también coompila los tests

