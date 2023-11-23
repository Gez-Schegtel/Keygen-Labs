#include <stdio.h> // For `printf()`
#include <stdlib.h> // For `exit()`
#include <stdbool.h> // To define boolean values such 'true' and 'false'
#include <string.h> // For `strcmp()`
#include <ctype.h> // For `tolower()`

/* Definimos macros para mejorar la compatibilidad entre sistemas Linux y Windows. */
#ifdef _WIN32
    // Windows
    #define PAUSE() system("pause")

    #define CLEAR_SCREEN() system("cls")

    #include <windows.h>
    #define SLEEP(x) Sleep(x * 1000)

#else
    // GNU/Linux
    #define CLEAR_SCREEN() system("clear")

    #include <unistd.h>
    #define SLEEP(x) sleep(x)

    #define PAUSE() \
        do { \
            printf("\nPresione una tecla para continuar... \n"); \
            while (getchar() != '\n'); \
        } while (0)
#endif

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

typedef struct reporte {
    int idProc;
    int tiempoRetorno;
    int tiempoEspera;
    struct reporte *prox;
} Reporte;

Proceso *primp, *p, *priml, *rl, *sl, *res;

Particion memoria[4];

Reporte *primrep, *rep, *resrep;

int cantProc, userTa, userTi, userTam, tresVeces, acumlTi;
int tiempoCiclo = 0, multiprog = 0, quantum = 0;

bool particionRequerida = false, fin = false;

int controlDeEntradas(char *myString, int min, int max) {
    /* Esta función sirve para controlar que el usuario ingrese los valores esperados. */
    int valor;
    bool bandit;

    do {
        /* Pide al usuario que ingrese un valor entero. */
        printf("Ingrese el %s.\n", myString);
        printf("Se aceptan valores comprendidos entre %d y %d.\n>> ", min, max);

        /* Intenta leer un entero desde la entrada estándar. */
        if (scanf("%d", &valor) != 1) {
            /* La entrada no fue un entero válido. */
            printf("\nError: Has ingresado un valor que no es númerico.\n\n");
            bandit = false;
            /* ¡Importante! Limpiar el buffer de entrada para evitar bucles infinitos. */
            while (getchar() != '\n');
        
        } else if (!(valor >= min && valor <= max)) {
            printf("\nError: has ingresado un valor numérico fuera del rango permitido.\n\n");
            bandit = false;
        } else {
            bandit = true;
        }
    } while (!bandit);

    return(valor);
}

void insertarOrdenado(Proceso **cabeza, Proceso *nuevo){
    /* Inserción ordenada de nodos en una lista. */
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

void cargaEncolada(){
    if (primrep == NULL){
        primrep = rep;
    } else {
        resrep->prox = rep;
    }
    resrep = rep;
}

void cargaManual(void){
    /* En esta función cargamos los procesos en una lista ordenada, ya sea de forma manual (pidiéndoselo al usuario) o automática (generando números aleatorios). */

    cantProc = controlDeEntradas("número de procesos que desee planificar", 1, 10);

    for (int i = 1; i <= cantProc; i++) {
        printf("\nProceso número: %d \n", i);

        p = (Proceso *)malloc(sizeof(Proceso)); /*De esta manera se crea un nuevo nodo.*/
        p->idProc = i;

        p->ta = controlDeEntradas("tiempo de arribo", 0, 20);
        p->ti = controlDeEntradas("tiempo de irrupción", 1, 20);
        acumlTi += p->ti;
        p->tr = p->ti;
        p->tam = controlDeEntradas("tamaño del proceso", 1, 250);
        
        insertarOrdenado(&primp, p);

        rep = (Reporte *)malloc(sizeof(Reporte));
        rep->idProc = i;
        rep->tiempoRetorno = 0;
        rep->tiempoEspera = 0;
        rep->prox = NULL;

        cargaEncolada();
    }
    CLEAR_SCREEN();
}

bool cargaArchivo(void) {
    FILE *archivo;
    char nombreArchivo[] = "procesos.csv"; // Nombre del archivo CSV
    Proceso procesos[10]; // Array para almacenar los procesos del archivo
    int contador = 0;

    // Abrir el archivo en modo lectura
    archivo = fopen(nombreArchivo, "r");

    if (archivo == NULL) { /* Si esta condición devuelve True quiere decir que no se pudo abrir el archivo. */
        return(false);
    }

    // Leer y descartar la primera línea (encabezados)
    char buffer[100];
    fgets(buffer, sizeof(buffer), archivo);

    // Leer el archivo línea por línea y asignar los valores a las variables
    while (contador < 10) {

        p = (Proceso *)malloc(sizeof(Proceso));
        if (p == NULL) {
            printf("Error: No se pudo asignar memoria para el nuevo nodo.\n");
            break;
        }

        // Leer los valores y asignarlos al nodo
        if (fscanf(archivo, "%d,%d,%d,%d\n", &p->idProc, &p->ta, &p->ti, &p->tam) != 4) {
            free(p); // Liberar la memoria asignada
            break;
        }

        acumlTi += p->ti;
        p->tr = p->ti;

        insertarOrdenado(&primp, p);

        /* Hacemos la carga para los reportes. */
        rep = (Reporte *)malloc(sizeof(Reporte));
        rep->idProc = p->idProc;
        rep->tiempoRetorno = 0;
        rep->tiempoEspera = 0;
        rep->prox = NULL;
        cargaEncolada();

        contador++;
    }

    // Cerrar el archivo
    fclose(archivo);
    return(true);
}

void menuOpcionesDeEntrada(void) {
    char respuesta[7];

    do {
        printf("Para cargar manualmente los procesos, ingrese 'manual'.\n");
        printf("Para leer los procesos desde el archivo 'procesos.csv', ingrese 'leer'.\n");
        printf("Si desea detener la operación, ingrese 'quit'.\n>> ");
        scanf("%s", respuesta);
        while (getchar() != '\n'); /* Limpiamos el buffer para evitar bucles infinitos. */

        /* Convertir la respuesta a minúsculas para hacer la comparación no sensible a mayúsculas. '\0' es el caracter NULL que indica el fin de un string. */
        for (int i = 0; respuesta[i] != '\0'; i++) {
            respuesta[i] = tolower(respuesta[i]);
        }

        if (strcmp(respuesta, "manual") == 0) {
            printf("\nCarga manual seleccionada.\n");
            SLEEP(2);
            CLEAR_SCREEN();
            cargaManual(); /* Nos lleva a la carga manual. */
            // cargaTesting();
            break;
        } else {
            if (strcmp(respuesta, "leer") == 0) {
                printf("\nCarga desde el archivo seleccionada.\n");
                SLEEP(2);
                CLEAR_SCREEN();
                if (cargaArchivo()){
                    break;
                } else {
                    printf("No se pudo abrir el archivo.\n");
                    printf("Asegúrese de tener el archivo 'procesos.csv' en el directorio del programa.\n");
                    PAUSE();
                    CLEAR_SCREEN();
                }
            } else { 
                if (strcmp(respuesta, "quit") == 0) {
                    exit(0);
                } else {
                    printf("\nRespuesta no válida.\n");
                    PAUSE();
                    CLEAR_SCREEN();
                }
            }
        }
    } while(1);
}

void recorridoListaInicial(Proceso *r){
    printf("Lista de procesos nuevos: \n");

    while (r != NULL){
        printf("Id del proceso: %d \n", r->idProc);
        printf("Tiempo de arribo: %d \n", r->ta);
        printf("Tiempo de irrupción: %d \n", r->ti);
        printf("Tamaño del proceso: %d \n", r->tam);
        
        r = r->prox;

        printf("\n");
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

    particionRequerida = false; /* Desmarcamos, ya que el proceso pudo ser asignado a memoria. */
}

bool noEstaAsignado(int idProc) {
    for (int i = 1; i <= 3; ++i) {
        if (memoria[i].idProcAsig == idProc) {
            return false;  /* idProc encontrado, no cumple la condición. */
        }
    }
    return true;  /* idProc no encontrado en ninguna estructura, cumple la condición. */
}

void best_fit(void) {

    if (noEstaAsignado(sl->idProc)) {

        if (sl->tam <= 60 && memoria[1].libre) {
            asignarEnMemoria(1, sl, particionRequerida);
        } else {
            if (sl->tam > 60 && sl->tam <= 120 && memoria[2].libre) {
                asignarEnMemoria(2, sl, particionRequerida);
            } else {
                if (sl->tam > 120 && sl->tam <= 250 && memoria[3].libre) {
                    asignarEnMemoria(3, sl, particionRequerida);
                } else {
                    // Esto controla si la partición que algún proceso requiere está ocupada.
                    particionRequerida = true;
                }
            }
        }
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
        memoria[i].libre = true;
        memoria[i].idProcAsig = 0;
        memoria[i].fragInt = 0;
    }
    particionRequerida = false;
}

void muestrasParciales(Proceso *r){
    printf("\nInstante número: %d \n", tiempoCiclo);
    if (r->tr > 0) {
        printf("Proceso en ejecución: %d \n", r->idProc);
    } else {
        printf("Proceso en ejecución: ~ \n");
    }
    
    mostrarMemoria();
}

void newToReady(void){
    if (priml == NULL){
        priml = primp;
        if (primp->prox != NULL){ //Cambio para hacer que si viene un solo proceso, no avance
            primp = primp->prox;
        } else {
            primp = NULL;
        };
        priml->prox = NULL;
        rl = priml;
        sl = priml; /* Esto queda así para no asignar varias veces sl para hacer la asignación en memoria. */
    } else {
        rl->prox = primp;
        rl = primp;
        if (primp->prox != NULL){
            primp = primp->prox;
        } else {
            primp = NULL;
        }
        rl->prox = NULL; /*Esto queda sin nada sólo en la último proceso de la cola*/
    }
}

void calcularReportes(Reporte *r){

    while (r != NULL){
        r->tiempoRetorno = tiempoCiclo - res->ta;
        r->tiempoEspera = r->tiempoRetorno - res->ti;
        r = r->prox;
    }
}

void verificarFinProceso(void) {
    if (quantum == 2 && priml->prox != NULL) {
        liberarMemoria();
        res = priml;
        priml = priml->prox;
        
        while (rl->prox != NULL){
            rl = rl->prox;
        }
        
        res->prox = NULL;
        
        /* Verificamos si nos queda tiempo de irrupción. */
        if (res->tr > 0){
            rl->prox = res;
        } else {
            if (res->tr == 0){
                multiprog--;

                calcularReportes(primrep);
            }
        }
        
        while (rl->prox != NULL){
            rl = rl->prox;
        }
    } else {
        if (quantum == 2 && priml->tr > 0 && priml->prox == NULL) {
            liberarMemoria();
        } else {
            /* Aquí terminó un proceso sin que termine el quantum, es decir, cuando es 1. */
            if (quantum < 2 && priml->tr == 0 && priml->prox != NULL){
                liberarMemoria();
                res = priml;
                priml = priml->prox;
                res->prox = NULL;
                quantum = 0;
                multiprog--;

                while (rl->prox != NULL){
                    rl = rl->prox;
                }

                calcularReportes(primrep);

            } else {
                if (priml->tr == 0 && priml->prox == NULL){
                    /* Aquí termina un proceso y es el último de todos. */
                    liberarMemoria();
                    fin = true;
                    
                    //calcularReportes(primrep);
                }
            }
        }
    }
}

char menuOpcionesDeFlujo(void){
    char respuesta[2];
    printf("\n");
    while(1){
        printf("Presione la tecla C para continuar. \n");
        printf("Presione la tecla T para dar un salto de tiempo. \n");
        printf("Presione la tecla F para para ir hasta el final. \n");
        printf("Presione la tecla S para salir. \n");
        printf(">> ");
        scanf("%s", respuesta);
        while (getchar() != '\n'); /* Limpiamos el buffer para evitar bucles infinitos. */

        /* Convertir la respuesta a minúsculas para hacer la comparación no sensible a mayúsculas. '\0' es el caracter NULL que indica el fin de un string. */
        for (int i = 0; respuesta[i] != '\0'; i++) {
            respuesta[i] = tolower(respuesta[i]);
        }
    
        if (strcmp(respuesta, "c") == 0){
            printf("\nContinuamos la ejecución al tiempo de ciclo siguiente.\n");
            return('c');
        } else {
            if (strcmp(respuesta, "t") == 0){
                printf("\n");
                return('t');
            } else {
                if (strcmp(respuesta, "f") == 0){
                    printf("Nos desplazamos hasta el último tiempo de ciclo.\n");
                    return('f');
                } else {
                    if (strcmp(respuesta, "s") == 0){
                        printf("\nDeteniendo la ejecución del programa.\n");
                        SLEEP(1);
                        exit(1);
                    } else {
                        printf("\nHas ingresado un valor incorrecto.\n");
                    }
                }
            }
        }
        printf("\n");
    }
}

void simulacion(void){
    int saltoTiempo = 0;
    int resTiempo = tiempoCiclo;
    bool final = false;

    while (1) { 

        while (primp != NULL && primp->ta == tiempoCiclo && multiprog < 5){

            newToReady();

            multiprog++;
        }
                
        tresVeces = 0;
        sl = priml;
        while (sl != NULL && tresVeces < 3) {
            best_fit();
            tresVeces++;
            if (sl->prox != NULL && !particionRequerida) {
                sl = sl->prox;
            }
        }
        
        /* Pequeño mensaje para dar aviso que empezó la planificación de los procesos. */
        if (tiempoCiclo == 0 && priml != NULL){
            printf("Comenzando la simulación... \n");
            SLEEP(2);
        }

        if (priml != NULL) {
            if ((resTiempo + saltoTiempo) == tiempoCiclo){
                muestrasParciales(priml);

                if (saltoTiempo == 0){
                    saltoTiempo = 1;
                }

                /* Esta condición controla que si el usuario eligió ir hasta el final, no se muestre más el menú de opciones del flujo de ejecución. */
                if (!final) {
                    char control = menuOpcionesDeFlujo();
                    if (control == 't') {
                        saltoTiempo = controlDeEntradas("salto de tiempo", 1, 20);
                    } else {
                        if (control == 'f') {
                            final = true;
                        }
                    }
                }
                resTiempo = tiempoCiclo;
            }
            priml->tr--;
        } else {
            printf("\nInstante número: %d\n", tiempoCiclo);
            printf("Proceso en ejecución: ~\n");
            mostrarMemoria();
        }

        quantum++;
        tiempoCiclo++;

        if (priml != NULL) {
            verificarFinProceso();
        }

        if (quantum == 2) {
            quantum = 0;
        }

        /* Esta parte verifica el fin del programa. */
        if (condiciones()) {
            muestrasParciales(priml);
            break;
        }
    }
}

void verReportes(Reporte *r){
    printf("\nReportes estadísticos de los procesos:");
    while (r != NULL) {
        printf("\nProceso número: %d\n", resrep->idProc);
        printf("Tiempo de retorno: %d \n", resrep->tiempoRetorno);
        printf("Tiempo de espera:  %d \n", resrep->tiempoEspera); 
        r = r->prox;   
    }
}

int main(void){
    CLEAR_SCREEN();
    printf("Este programa es un simulador de asignación de memoria y gestión de procesos. Se permiten hasta 10 procesos con un tamaño de 250 como máximo. \n\n");

    menuOpcionesDeEntrada();

    recorridoListaInicial(primp);

    iniciarArreglo();

    simulacion();

    verReportes(primrep);

    return(0);
};
