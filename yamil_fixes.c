#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

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

Proceso *primp, *p, *priml, *rl, *sl, *res;

Particion memoria[4];

int cantProc, userTa, userTi, userTam, tresVeces, acumlTi, tiempoCiclo = 0, multiprog = 0, quantum = 0;

bool particionRequerida = false, fin = false;

void numProcesoUsuario(void){
    printf("Ingrese el número de procesos que desee planificar: ");
    scanf("%d", &cantProc);

    while (cantProc > 10 || cantProc < 1) {
        printf("Ha ingresado un número inválido de procesos. Ingrese un número del 1 hasta 10.\n >> ");
        scanf("%d", &cantProc);
    }
}

void datosProcesoUsuario(int i){
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
    
    acumlTi += userTi;

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

    while (actual != NULL && actual->ta <= nuevo->ta) {
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

void recorridoListaInicial(Proceso *r){
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

void generacionProcesosManual(void){
    /*Lo siguiente es equivalente a la definición de "puntero a Proceso".*/
    
    numProcesoUsuario();

    for (int i = 1; i <= cantProc; i++) {
        datosProcesoUsuario(i);

        p = (Proceso *)malloc(sizeof(Proceso)); /*De esta manera se crea un nuevo nodo.*/
        p->idProc = i;
        p->ta = userTa;
        p->ti = userTi;
        p->tam = userTam;
        p->tr = userTi;
        
        insertarOrdenado(&primp, p);
    }
}

void iniciarArreglo(void){
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

void mostrarMemoria(void){
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

void asignarEnMemoria(int index, Proceso *sl, bool particionRequerida) {
    memoria[index].idProcAsig = sl->idProc;
    memoria[index].fragInt = memoria[index].tamPart - sl->tam;
    memoria[index].libre = false;

    // Desmarcamos, ya que el proceso pudo ser asignado a memoria
    particionRequerida = false;
}

void best_fit(void) {
    // Algoritmo Best Fit
    if (rl->tam <= 60 && memoria[1].libre) {
        asignarEnMemoria(1, rl, particionRequerida);
    } else if (rl->tam > 60 && rl->tam <= 120 && memoria[2].libre) {
        asignarEnMemoria(2, rl, particionRequerida);
    } else if (rl->tam > 120 && rl->tam <= 250 && memoria[3].libre) {
        asignarEnMemoria(3, rl, particionRequerida);
    } else {
        // Esto controla si la partición que algún proceso requiere está ocupada
        particionRequerida = true;
    }
}

bool condiciones(void) {
    if (memoria[1].libre && memoria[2].libre && memoria[3].libre && fin && primp == NULL){
        return(true);
    } else {
        return(false);
    }
}

void liberarMemoria(void) {
    for (int i = 1; i <= 3; i++) {
        if (priml->idProc == memoria[i].idProcAsig) { //liberando memoria 
            memoria[i].libre = true;
            memoria[i].idProcAsig = 0;
            memoria[i].fragInt = 0;
        }
    }
}


void muestrasParciales(Proceso *r){
    printf("\nInstante número: %d \n", tiempoCiclo);
    if (r->tr > 0) {
        printf("Proceso en ejecución: %d \n", r->idProc);
    } else {
        printf("Proceso en ejecución: no hay perrito \n");
    }
    
    mostrarMemoria();

    // for (int i = 1; i < 4; i++){
    //     printf("Id de partición: %d \n", i);
    //     printf("Tamaño: %d \n", memoria[i].tamPart);
    //     printf("Dirección de comienzo: %d \n", memoria[i].dirCom);

    //     if (!memoria[i].libre){
    //         printf("Proceso asignado: %d \n", memoria[i].idProcAsig);
    //         printf("Fragmentación interna: %d \n", memoria[i].tamPart - r->tam);
    //     } else {
    //         printf("Proceso asignado: ∅ \n");
    //         printf("Fragmentación interna: ∅ \n");
    //     }
    // }
}

void randomAccessMemory(void){
    if (priml == NULL){
        //printf("priml == NULL\n");
        priml = primp;
        if (primp->prox != NULL){ //Cambio para hacer que si viene un solo proceso, no avance
            primp = primp->prox;
        } else {
            primp = NULL;
            free(primp);
        };
        priml->prox = NULL;
        rl = priml;
        sl = priml; /* Esto queda así para no asignar varias veces sl para hacer la asignación en memoria. */
    } else {
        //printf("priml != NULL\n");
        rl->prox = primp;
        rl = primp;
        if (primp->prox != NULL){
            primp = primp->prox;
        } else {
            primp = NULL;
            free(primp);
        }
        rl->prox = NULL; /*Esto queda alpedo sólo en la último proceso de la cola*/
        // sl = rl;
    }
    
    if (sl != NULL && !particionRequerida) {
        best_fit();
        if (particionRequerida) {
            sl = rl;
        }
        
    }
}

int main(void){
    printf("Este programa es un simulador de asignación de memoria y gestión de procesos. Se permiten hasta 10 procesos con un tamaño de 250 como máximo. \n\n");

    generacionProcesosManual();

    recorridoListaInicial(primp);

    iniciarArreglo();
    
    while (1) {        
        while (primp != NULL && primp->ta == tiempoCiclo && multiprog < 5){

            randomAccessMemory();

            multiprog++;
        }
        
        muestrasParciales(priml);

        tiempoCiclo++;
        quantum++;
        priml->tr--;
    
        if (quantum == 2 && priml->prox != NULL) {
            liberarMemoria();
            res = priml;
            priml = priml->prox;
            res->prox = NULL;
            quantum = 0;
            
            /* Verificamos si nos queda tiempo de irrupción. */
            if (res->tr > 0){
                rl->prox = res;
            } else {
                if (rl->tr == 0){
                    multiprog--;
                }
            }

        } else {
            // Aquí terminó un proceso sin que termine el quantum, es decir, cuando es 1
            if (quantum < 2 && priml->tr == 0 && priml->prox != NULL){
                liberarMemoria();
                res = priml;
                priml = priml->prox;
                res->prox = NULL;
                multiprog--;
            } else {
                if (priml->tr == 0 && priml->prox == NULL){
                    // Aquí termina un proceso y es el ÚLTIMO de todos
                    liberarMemoria();
                    fin = true;
                    //priml = NULL; // Hace que "deje de apuntar" a la dirección que originalmente apuntaba
                    //rl = NULL;
                    //res = NULL;
                    
                }
            }
        }

        if (condiciones()) {
            break;
        }
    }
    
    return 0;
}

