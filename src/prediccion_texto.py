import sys
import random
import json


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

def contar_ocurrencias(oracion, texto):
    ocurrencias = 0
    for i in range(len(texto)):
        if texto[i] == oracion:
            ocurrencias += 1
    return ocurrencias

def subcadenas(oracion, texto):
    subcadenas = []
    for i in range(len(oracion)):
        subcadenas.append(oracion[i:])
    return subcadenas

def modelo_oracion(oracion, texto):
    modelo = {}
    for subcadena in subcadenas(oracion, texto):
        ocurrencias = 0
        modelo[tuple(subcadena)] = 0 
    return modelo

def generar_modelos(frases, texto):
    modelos = {}
    for frase in frases:
        modelo = modelo_oracion(frase, texto)
        print(modelo)
        if modelo:
            modelos[tuple(frase)] = modelo
    return modelos


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
        print("ERROR: No se encontró el archivo " + frases_path, file=sys.stderr)
        sys.exit(1)


def main():
    try:
        nombre = sys.argv[1]
    except IndexError:
        print("ERROR: No se ingresó el nombre del archivo", file=sys.stderr)
        sys.exit(1)
    entrada = get_entrada(nombre)
    frases = leer_frases(nombre)
    modelos = generar_modelos(frases, entrada)
    with open('debugg.json', 'w') as f:
        modelos_str = {str(k): {str(sub_k): sub_v for sub_k, sub_v in v.items()} for k, v in modelos.items()}
        json.dump({"entrada": entrada, "frases": frases, "modelos": modelos_str}, f, indent=4)
    

if __name__ == "__main__":
    main()