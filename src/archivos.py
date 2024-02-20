
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

def escribir_salida(nombre, frases):
    try:
        salida = open("Salidas/" + nombre + ".txt", "w")
        for oracion in frases:
            salida.write(" ".join(oracion) + "\n")
        salida.close()
    except FileNotFoundError:
        print("ERROR: No se encontró el archivo " + nombre, file=sys.stderr)
        sys.exit(1)