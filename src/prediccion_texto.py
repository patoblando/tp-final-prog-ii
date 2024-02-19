import sys
import random
import json
from collections import defaultdict

SIG_PALABRA = "sig"
PREV_PALABRA = "prev"
DEBUGG = True

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

def generar_ngrama(words, n):
    return [tuple(words[i:i+n]) for i in range(len(words)-n+1)]

def get_palabra_modelo(palabra, direccion, modelo):
    if modelo[palabra]:
        return max(modelo[palabra][direccion].items(), key=lambda item: item[1])
    return None, 0

def predecir(oracion, modelo, peso_ant = 0.66, peso_sig = 0.33):
    idx = oracion.index("_")
    palabra_sig_apariciones = 0
    palabra_prev_apariciones = 0

    # Hay palabra previa
    if idx > 0: 
        prev_word = oracion[idx-1]
        palabra_predict_prev, palabra_prev_apariciones = get_palabra_modelo(prev_word, SIG_PALABRA, modelo)

    # Hay palabra siguiente
    if idx < len(oracion) - 1: 
        sig_word = oracion[idx+1]
        palabra_predict_sig, palabra_sig_apariciones = get_palabra_modelo(sig_word, PREV_PALABRA, modelo)

    # La palabra no esta en mi texto de entrenamiento
    if palabra_prev_apariciones == 0 and palabra_sig_apariciones == 0:
        palabra_random = random.choice(list(modelo.keys()))
        # Devuelvo una palabra al azar
        return palabra_random  if not DEBUGG else  "r_" + palabra_random 

    if palabra_prev_apariciones * peso_ant > palabra_sig_apariciones * peso_sig:
        return palabra_predict_prev
    else:
        return palabra_predict_sig
        

def entrenar_modelo(texto):
    modelo = defaultdict(lambda: defaultdict(lambda: defaultdict(int)))
    for oracion in texto:
        for w1, w2 in generar_ngrama(   oracion, 2):
            modelo[w1][SIG_PALABRA][w2] += 1
            modelo[w2][PREV_PALABRA][w1] += 1
    return modelo

def predecir_oraciones(frases, modelo):
    for oracion in frases:
        guion_idx = oracion.index("_")
        oracion[guion_idx] = "_" + predecir(oracion, modelo) + "_"
        if DEBUGG: print(oracion)
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
    modelos = entrenar_modelo(entrada)
    predecir_oraciones(frases, modelos)
    if DEBUGG:
        with open('debugg.json', 'w') as f:
            modelos_str = {str(k): {str(sub_k): sub_v for sub_k, sub_v in v.items()} for k, v in modelos.items()}
            json.dump({"modelos": modelos_str, "frases": frases}, f, indent=4)
    

if __name__ == "__main__":
    main()