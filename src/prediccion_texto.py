import sys
import random
import json
from collections import defaultdict

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

def predecir(oracion, modelo):
    idx = oracion.index("_")
    pred_prev_value = 0
    pred_sig_value = 0
    if idx > 0:
        prev_word = oracion[idx-1]
        if modelo[prev_word]:
            pred_sig_key, pred_sig_value = max(modelo[prev_word]["sig"].items(), key=lambda item: item[1])
    if idx < len(oracion) - 1:
        sig_word = oracion[idx+1]
        if modelo[sig_word]:
            pred_prev_key, pred_prev_value = max(modelo[sig_word]["prev"].items(), key=lambda item: item[1])
    if pred_sig_value == 0 and pred_prev_value == 0:
        return "unknown"
    return pred_sig_key if pred_sig_value > pred_prev_value else pred_prev_key
        

def entrenar_modelo(texto):
    modelo = defaultdict(lambda: defaultdict(lambda: defaultdict(int)))
    for oracion in texto:
        for w1, w2 in generar_ngrama(oracion, 2):
            modelo[w1]["sig"][w2] += 1
            modelo[w2]["prev"][w1] += 1
    return modelo

def predecir_oraciones(frases, modelo):
    for oracion in frases:
        # predecir(oracion, modelo)
        print("La predicción para la oración", oracion, "es", predecir(oracion, modelo))

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
    with open('debugg.json', 'w') as f:
        modelos_str = {str(k): {str(sub_k): sub_v for sub_k, sub_v in v.items()} for k, v in modelos.items()}
        json.dump({"modelos": modelos_str, "frases": frases}, f, indent=4)
    

if __name__ == "__main__":
    main()