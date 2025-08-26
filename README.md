# DISEÑO E IMPLEMENTACIÓN DE UN AFD (PYTHON Y C)

Este proyecto implementa un Autómata Finito Determinista (AFD) en dos lenguajes: Python y C. A partir de un archivo de configuración que define estados, alfabeto, transiciones y estados de aceptación, el programa procesa un conjunto de cadenas de prueba y determina el recorrido de estados y si cada cadena es aceptada o rechazada.

## Python

En la versión en Python se implementa un Autómata Finito Determinista (AFD) que lee la configuración del autómata y un conjunto de cadenas desde archivos de texto, y luego muestra el recorrido de estados junto con la aceptación o rechazo de cada cadena.

<pre>

def cargar_afd(archivo):
    with open(archivo, 'r') as f:
        lineas = [l.strip() for l in f if l.strip() and not l.startswith("#")]

    estados = set(lineas[0].split("=")[1].replace("{", "").replace("}", "").split(","))
    estados = {e.strip() for e in estados if e.strip()}

    alfabeto = set(lineas[1].split("=")[1].replace("{", "").replace("}", "").split(","))
    alfabeto = {a.strip() for a in alfabeto if a.strip()}

    estado_inicial = lineas[2].strip()

    estados_aceptacion = set(lineas[3].split("=")[1].replace("{", "").replace("}", "").split(","))
    estados_aceptacion = {e.strip() for e in estados_aceptacion if e.strip()}

    transiciones = {}
    for regla in lineas[4:]:
        izq, der = regla.split("->")
        estado, simbolo = izq.split(",")
        transiciones[(estado.strip(), simbolo.strip())] = der.strip()

    return estados, alfabeto, estado_inicial, estados_aceptacion, transiciones


def procesar_cadena(cadena, estado_inicial, transiciones, estados_aceptacion):
    estado_actual = estado_inicial
    recorrido = [estado_actual]

    for simbolo in cadena:
        if (estado_actual, simbolo) in transiciones:
            estado_actual = transiciones[(estado_actual, simbolo)]
            recorrido.append(estado_actual)
        else:
            recorrido.append("ERROR")
            return recorrido, False

    return recorrido, estado_actual in estados_aceptacion


if __name__ == "__main__":
    estados, alfabeto, estado_inicial, estados_aceptacion, transiciones = cargar_afd("config.txt")

    with open("cadenas.txt", "r") as f:
        cadenas = [linea.strip() for linea in f if linea.strip()]

    for cadena in cadenas:
        recorrido, aceptada = procesar_cadena(cadena, estado_inicial, transiciones, estados_aceptacion)
        print(f"Cadena: {cadena}")
        print(f"Recorrido: {' -> '.join(recorrido)}")
        print("Resultado:", "ACEPTADA" if aceptada else "NO ACEPTADA")
        print("------")

</pre>

A continuación, podemos ver la salida por consola, donde nos muestra la cadena, el recorrido que esta hace, y por último si, con base en las reglas, es aceptada o no.

## C
