import sys
import random


def predecir():
    pass

def get_entrada(nombre):
    try:
        entrada = open("Entradas/" + nombre + ".txt", "r")

        texto = entrada.read().split("\n")
        for i in range(len(texto)):
            texto[i] = texto[i].split(" ")

        return texto
    
    except FileNotFoundError:
        print("ERROR: No se encontró el archivo " + nombre, file=sys.stderr)
        sys.exit(1)
    except IndexError:
        print("ERROR: No se ingresó el nombre del archivo", file=sys.stderr)
        sys.exit(1)

def generar_modelo(oracion, texto):
    pass

def predecir(oracion, modelo):
    pass

#Lee las oraciones de Frases/{{arg}}.txt y las devuelve en una lista de listas (frases)
def leer_frases(nombre):
    try:
        frases_path = "Frases/" + nombre + ".txt"
        archivo = open(frases_path, "r")
        frases = archivo.read().split("\n")
        for i in range(len(frases)):
            frases[i] = frases[i].split(" ")
        archivo.close()
        return frases
    except FileNotFoundError:
        print("ERROR: No se encontró el archivo", file=sys.stderr)
        sys.exit(1)

# TODO: Borrar las palabras vacias de entradas y frases
def main():
    nombre = sys.argv[1]
    entrada = get_entrada(nombre)
    frases = leer_frases(nombre)
    print(entrada) # TODO: Printear entradas y frases a un json asi lo puedo ver lindo
    print(frases)

if __name__ == "__main__":
    main()