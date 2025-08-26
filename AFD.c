#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ESTADOS 50
#define MAX_TRANSICIONES 200
#define MAX_CADENA 200

typedef struct {
    char estado[20];
} Estado;

typedef struct {
    char origen[20];
    char simbolo;
    char destino[20];
} Transicion;

Estado estados[MAX_ESTADOS];
int numEstados = 0;

char alfabeto[50];
int numSimbolos = 0;

char estadoInicial[20];
Estado estadosAceptacion[MAX_ESTADOS];
int numAceptacion = 0;

Transicion transiciones[MAX_TRANSICIONES];
int numTransiciones = 0;

void cargarAFD(const char *archivo) {
    FILE *f = fopen(archivo, "r");
    if (!f) {
        perror("No se pudo abrir config.txt");
        exit(1);
    }

    char linea[256];
    while (fgets(linea, sizeof(linea), f)) {
        if (linea[0] == '#' || strlen(linea) < 2) continue;

        if (linea[0] == 'Q') {
            char *p = strchr(linea, '{');
            char *q = strchr(linea, '}');
            *q = '\0';
            p++;
            char *token = strtok(p, ",");
            while (token) {
                strcpy(estados[numEstados++].estado, token);
                token = strtok(NULL, ",");
            }
        } else if (strncmp(linea, "ALFABETO", 8) == 0) {
            char *p = strchr(linea, '{');
            char *q = strchr(linea, '}');
            *q = '\0';
            p++;
            char *token = strtok(p, ",");
            while (token) {
                alfabeto[numSimbolos++] = token[0];
                token = strtok(NULL, ",");
            }
        } else if (linea[0] == 'F') {
            char *p = strchr(linea, '{');
            char *q = strchr(linea, '}');
            *q = '\0';
            p++;
            char *token = strtok(p, ",");
            while (token) {
                strcpy(estadosAceptacion[numAceptacion++].estado, token);
                token = strtok(NULL, ",");
            }
        } else if (strchr(linea, ',') && strchr(linea, '-')) {
            char izq[50], der[50];
            sscanf(linea, "%[^-]->%s", izq, der);
            char *coma = strchr(izq, ',');
            *coma = '\0';
            strcpy(transiciones[numTransiciones].origen, izq);
            transiciones[numTransiciones].simbolo = *(coma + 1);
            strcpy(transiciones[numTransiciones].destino, der);
            numTransiciones++;
        } else {
            strcpy(estadoInicial, linea);
            estadoInicial[strcspn(estadoInicial, "\n")] = 0;
        }
    }

    fclose(f);
}

const char* mover(const char *estado, char simbolo) {
    for (int i = 0; i < numTransiciones; i++) {
        if (strcmp(transiciones[i].origen, estado) == 0 && transiciones[i].simbolo == simbolo) {
            return transiciones[i].destino;
        }
    }
    return NULL;
}

int esAceptacion(const char *estado) {
    for (int i = 0; i < numAceptacion; i++) {
        if (strcmp(estadosAceptacion[i].estado, estado) == 0) return 1;
    }
    return 0;
}

void procesarCadena(const char *cadena) {
    const char *estado = estadoInicial;
    printf("Cadena: %s\n", cadena);
    printf("Recorrido: %s", estado);

    for (int i = 0; i < strlen(cadena); i++) {
        estado = mover(estado, cadena[i]);
        if (!estado) {
            printf(" -> ERROR\nResultado: NO ACEPTADA\n------\n");
            return;
        }
        printf(" -> %s", estado);
    }
    printf("\nResultado: %s\n------\n", esAceptacion(estado) ? "ACEPTADA" : "NO ACEPTADA");
}

int main() {
    cargarAFD("config.txt");

    FILE *f = fopen("cadenas.txt", "r");
    if (!f) {
        perror("No se pudo abrir cadenas.txt");
        exit(1);
    }

    char cadena[MAX_CADENA];
    while (fgets(cadena, sizeof(cadena), f)) {
        cadena[strcspn(cadena, "\n")] = 0;
        if (strlen(cadena) > 0) {
            procesarCadena(cadena);
        }
    }

    fclose(f);
    return 0;
}

