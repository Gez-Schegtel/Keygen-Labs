#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct proceso {
    int idProc;
    int ta; // tiempo de arribo
    int ti; // tiempo de irrupción
    int tam; // tamaño
    int tr; // tiempo restante
    struct proceso *prox;
} Proceso; /* Proceso queda definido como un tipo de dato. */

typedef struct particion {
    int idPart;
    int dirCom;
    int tamPart;
    int idProcAsig;
    int fragInt;
    bool libre;
} Particion;

Proceso *primp, *p;

Particion memoria[4];

int cantProc, userTa, userTi, userTam, acuml= 0, tiempoCiclo = 0, multiprog = 0;

bool particionRequerida;

void userProcessNumber(void){
    printf("Ingrese el número de procesos que desee planificar: ");
    scanf("%d", &cantProc);

    while (cantProc > 10 || cantProc < 1) {
        printf("Ha ingresado un número inválido de procesos. Ingrese un número del 1 hasta 10.\n >> ");
        scanf("%d", &cantProc);
    }
}

void userProcessData(int i){
    printf("\nProceso nro. %d \n", i);
    
    printf("Ingrese el tiempo de arribo >> "); /*Puede ser cero*/
    scanf("%d", &userTa);
    
    while (userTa > 1000 || userTa < 0){
        printf("Ha ingresado un tiempo de arribo incorrecto. Ingrese un valor del 0 al 1000. \n");
        scanf("%d", &userTa);
    }

    printf("Ingrese el tiempo de irrupción >> ");
    scanf("%d", &userTi);

    while (userTi > 1000 || userTi < 1){
        printf("Ha ingresado un tiempo de irrupción incorrecto. Ingrese un valor del 1 al 1000. \n");
        scanf("%d", &userTi);
    }
    
    acuml += userTi;

    printf("Ingrese el tamaño del proceso >> ");
    scanf("%d", &userTam);

    while (userTam > 250 || userTam < 1){
        printf("Ha ingresado un tamaño incorrecto para el proceso. Se permiten valores de 1 hasta 250. \n");
        scanf("%d", &userTam);
    }
}

void insertarOrdenado(Proceso **cabeza, Proceso *nuevo){
    Proceso *actual = *cabeza;
    Proceso *anterior = NULL;

    while (actual != NULL && actual->ta < nuevo->ta) {
        anterior = actual;
        actual = actual->prox;
    }

    if (anterior == NULL) {
        nuevo->prox = *cabeza;
        *cabeza = nuevo;
    } else {
        anterior->prox = nuevo;
        nuevo->prox = actual;
    }
}

void recorridoLista(Proceso *r){
    // system("clear");
    printf("Lista de procesos nuevos: \n");

    while (r != NULL){
        printf("\nId del proceso: %d \n", r->idProc);
        printf("Tiempo de arribo: %d \n", r->ta);
        printf("Tiempo de irrupción: %d \n", r->ti);
        printf("Tamaño del proceso: %d \n", r->tam);
        
        r = r->prox;
    }
}

void generacionProcesos(){
    /*Lo siguiente es equivalente a la definición de "puntero a Proceso".*/
    
    userProcessNumber();

    for (int i = 1; i <= cantProc; i++) {
        userProcessData(i);

        p = (Proceso *)malloc(sizeof(Proceso)); /*De esta manera se crea un nuevo nodo.*/
        p->idProc = i;
        p->ta = userTa;
        p->ti = userTi;
        p->tam = userTam;
        p->tr = userTi;
        
        insertarOrdenado(&primp, p);
    }
}

void iniciarArreglo(){
    int i;

    for (i = 0; i < 4; i++) {
        memoria[i].idPart = i;
        memoria[i].libre = true;
    }

    memoria[0].dirCom = 0;
    memoria[0].tamPart = 100;

    memoria[1].dirCom = 100;
    memoria[1].tamPart = 60;

    memoria[2].dirCom = 160;
    memoria[2].tamPart = 120;

    memoria[3].dirCom = 280;
    memoria[3].tamPart = 250;
}

void recorridoArreglo(){
    // system("clear");
    printf("\nParticiones de memoria: \n");
    for (int i = 0; i < 4; i++) {
    printf("Particion %d - ID: %d, DirCom: %d, TamPart: %d, IDProcAsig: %d, FragInt: %d, Libre: %s\n",
            i + 1,
            memoria[i].idPart,
            memoria[i].dirCom,
            memoria[i].tamPart,
            memoria[i].idProcAsig,
            memoria[i].fragInt,
            memoria[i].libre ? "true" : "false"); /* De esta manera se puede mostrar por pantalla "true" o "false", ya que por defecto se muestran los valores 1 o 0 para estos valores, respectivamente. */
    }
};

int main(void){
    printf("Este programa es un simulador de asignación de memoria y gestión de procesos. Se permiten hasta 10 procesos con un tamaño de 250 como máximo. \n\n");

    generacionProcesos();

    recorridoLista(primp);

    iniciarArreglo();

    recorridoArreglo();
    
    return 0;
};
