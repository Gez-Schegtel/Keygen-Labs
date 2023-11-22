#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <time.h>

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
} Reporte;

Proceso *primp, *p, *priml, *rl, *sl, *res;

Particion memoria[4];

int cantProc, userTa, userTi, userTam, tresVeces, acumlTi, tiempoCiclo = 0, multiprog = 0, quantum = 0, tiempoRet;
int arch = 1;
bool particionRequerida = false, fin = false;

Reporte paraMostrar[10];

void iniReporte(){
    for (int i = 1; i <= 10; i++){
        paraMostrar[i].idProc = 0;
        paraMostrar[i].tiempoRetorno = 0;
        paraMostrar[i].tiempoEspera = 0;
    }
}

int controlDeEntradas(char *myString, int min, int max) {
    int valor;
    bool bandit;

    do {
        /* Pide al usuario que ingrese un valor entero. */
        printf("\nIngrese el %s.\n", myString);
        printf("Se aceptan valores comprendidos entre %d y %d.\n: ", min, max);

        /* Intenta leer un entero desde la entrada estándar. */
        if (scanf("%d", &valor) != 1) {
            /* La entrada no fue un entero válido. */
            printf("\nError: Has ingresado un valor que no es númerico.\n");
            bandit = false;
            /* ¡Importante! Limpiar el buffer de entrada para evitar bucles infinitos. */
            while (getchar() != '\n');
        
        } else if (!(valor >= min && valor <= max)) {
            printf("\nError: has ingresado un valor numérico fuera del rango permitido.\n");
            bandit = false;
        } else {
            bandit = true;
        }
    } while (!bandit);

    return(valor);
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

void cargaTesting(void){
    /*Lo siguiente es equivalente a la definición de "puntero a Proceso".*/
    
    p = (Proceso *)malloc(sizeof(Proceso)); /*De esta manera se crea un nuevo nodo.*/
    p->idProc = 1;
    p->ta = 0;
    p->ti = 4;
    p->tam = 100;
    p->tr = p->ti;
    
    insertarOrdenado(&primp, p);

    p = (Proceso *)malloc(sizeof(Proceso)); /*De esta manera se crea un nuevo nodo.*/
    p->idProc = 2;
    p->ta = 0;
    p->ti = 5;
    p->tam = 50;
    p->tr = p->ti;
    
    insertarOrdenado(&primp, p);

    p = (Proceso *)malloc(sizeof(Proceso)); /*De esta manera se crea un nuevo nodo.*/
    p->idProc = 3;
    p->ta = 1;
    p->ti = 2;
    p->tam = 200;
    p->tr = p->ti;
    
    insertarOrdenado(&primp, p);

    p = (Proceso *)malloc(sizeof(Proceso)); /*De esta manera se crea un nuevo nodo.*/
    p->idProc = 4;
    p->ta = 2;
    p->ti = 7;
    p->tam = 10;
    p->tr = p->ti;
    
    insertarOrdenado(&primp, p);

    p = (Proceso *)malloc(sizeof(Proceso)); /*De esta manera se crea un nuevo nodo.*/
    p->idProc = 5;
    p->ta = 4;
    p->ti = 10;
    p->tam = 100;
    p->tr = p->ti;
    
    insertarOrdenado(&primp, p);

    p = (Proceso *)malloc(sizeof(Proceso)); /*De esta manera se crea un nuevo nodo.*/
    p->idProc = 6;
    p->ta = 6;
    p->ti = 4;
    p->tam = 100;
    p->tr = p->ti;
    
    insertarOrdenado(&primp, p);

}

void cargaManual(void){
    /*Lo siguiente es equivalente a la definición de "puntero a Proceso".*/
    
    cantProc = controlDeEntradas("número de procesos que desee planificar", 1, 10);

    for (int i = 1; i <= cantProc; i++) {
        printf("\nProceso nro. %d \n", i);

        p = (Proceso *)malloc(sizeof(Proceso)); /*De esta manera se crea un nuevo nodo.*/
        p->idProc = i;
        paraMostrar[i].idProc = i;
        paraMostrar[i].tiempoRetorno = 0;
        paraMostrar[i].tiempoEspera = 0;
        p->ta = controlDeEntradas("tiempo de arribo", 0, 20);
        p->ti = controlDeEntradas("tiempo de irrupción", 1, 20);
        acumlTi += p->ti;
        p->tr = p->ti;
        p->tam = controlDeEntradas("tamaño del proceso", 1, 250);
        
        insertarOrdenado(&primp, p);
    }
}

int generacionAleatoria(int min, int max){
    srand(time(NULL)); /* Hace que la función rand() genere números distintos con el pasar del tiempo. */
	int numeroAleatorio = min + rand() % max; /* Generamos un número aleatorio entre un mínimo y un máximo solicitado. */
    sleep(1); /* Dejamos que pase un tiempo para que no se repitan los valores. */
    return(numeroAleatorio);
}

void cargaAutomatica(void){
    /*Lo siguiente es equivalente a la definición de "puntero a Proceso".*/
    
    cantProc = controlDeEntradas("número de procesos que desee planificar", 1, 10);
        
    for (int i = 1; i <= cantProc; i++) {
        printf("\nGenerando proceso nro. %d \n", i);

        p = (Proceso *)malloc(sizeof(Proceso)); /*De esta manera se crea un nuevo nodo.*/
        p->idProc = i;
        paraMostrar[i].idProc = i;
        p->ta = generacionAleatoria(0, 20);
        p->ti = generacionAleatoria(1, 20);
        acumlTi += p->ti;
        p->tr = p->ti;
        p->tam = generacionAleatoria(1, 250);
        
        insertarOrdenado(&primp, p);
    }
}

int cargaArchivo(void) {
    FILE *archivo;
    char nombreArchivo[] = "procesos.csv"; // Nombre del archivo CSV
    Proceso procesos[10]; // Array para almacenar los procesos del archivo
    int contador = 0;

    // Abrir el archivo en modo lectura
    archivo = fopen(nombreArchivo, "r");

    if (archivo == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return 0;
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

        contador++;
    }

    // Cerrar el archivo
    fclose(archivo);
    return 1;
}

void menu(void) {
    char respuesta[7];

    do {
        printf("Para cargar manualmente los procesos, ingrese 'manual'. \nPara generar los procesos de manera automática, ingrese 'auto'. \nPara leer los procesos desde el archivo 'procesos.csv', ingrese 'leer'. \nSi desea detener la operación, ingrese 'quit'. \n:");
        scanf("%s", respuesta);

        /* Convertir la respuesta a minúsculas para hacer la comparación no sensible a mayúsculas. '\0' es el caracter NULL que indica el fin de un string. */
        for (int i = 0; respuesta[i] != '\0'; i++) {
            respuesta[i] = tolower(respuesta[i]);
        }

        // Comparar cadenas sin usar strcmp
        if (strcmp(respuesta, "manual") == 0) {
            printf("Cargando manualmente...\n");
            cargaManual();
            break;
        } else if (strcmp(respuesta, "auto") == 0) {
            printf("Cargando automáticamente...\n");
            cargaAutomatica();
            break;
        } else if (strcmp(respuesta, "leer") == 0) {
            printf("Cargando desde archivo...\n");
            arch = cargaArchivo();
            break;
        } else if (strcmp(respuesta, "quit") == 0) {
            printf("Deteniendo la ejecución...\n");
            exit(0);
        } else {
            printf("Respuesta no válida.\nEjecutando el programa nuevamente... \n");
            sleep(2);
            system("clear");
        }
    } while(1);
}

void recorridoListaInicial(Proceso *r){
    printf("Lista de procesos nuevos: \n");

    while (r != NULL){
        printf("\nId del proceso: %d \n", r->idProc);
        printf("Tiempo de arribo: %d \n", r->ta);
        printf("Tiempo de irrupción: %d \n", r->ti);
        printf("Tamaño del proceso: %d \n", r->tam);
        
        r = r->prox;
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

    // Desmarcamos, ya que el proceso pudo ser asignado a memoria
    particionRequerida = false;
}

void best_fit(void) {

    if (memoria[1].idProcAsig != sl->idProc && memoria[2].idProcAsig != sl->idProc && memoria[3].idProcAsig != sl->idProc) {

        if (sl->tam <= 60 && memoria[1].libre) {
            asignarEnMemoria(1, sl, particionRequerida);
        } else {
            if (sl->tam > 60 && sl->tam <= 120 && memoria[2].libre) {
                asignarEnMemoria(2, sl, particionRequerida);
            } else {
                if (sl->tam > 120 && sl->tam <= 250 && memoria[3].libre) {
                    asignarEnMemoria(3, sl, particionRequerida);
                } else {
                    // Esto controla si la partición que algún proceso requiere está ocupada
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
        printf("Proceso en ejecución: No hay procesos \n");
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

void verificarFinProceso(void) {
    if (quantum == 2 && priml->prox != NULL) {
        liberarMemoria();
        res = priml;
        priml = priml->prox;
        
        while (rl->prox != NULL){
            rl = rl->prox;
        }
        
        res->prox = NULL;
        //quantum = 0;
        
        /* Verificamos si nos queda tiempo de irrupción. */
        if (res->tr > 0){
            rl->prox = res;
        } else {
            if (res->tr == 0){
                multiprog--;

                for (int i = 0; i <= cantProc; i++) {
                    if (res->idProc == paraMostrar[i].idProc) {
                        tiempoRet = tiempoCiclo - res->ta;
                        paraMostrar[i].tiempoRetorno = tiempoRet;
                        paraMostrar[i].tiempoEspera = tiempoRet - res->ti;
                    }
                }
            }
        }
        
        while (rl->prox != NULL){
            rl = rl->prox;
        }
    } else {
        if (quantum == 2 && priml->tr > 0 && priml->prox == NULL) {
            //quantum = 0;
            liberarMemoria();
        } else {
            // Aquí terminó un proceso sin que termine el quantum, es decir, cuando es 1
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

                for (int i = 0; i <= cantProc; i++) {
                    if (res->idProc == paraMostrar[i].idProc) {
                        tiempoRet = tiempoCiclo - res->ta;
                        paraMostrar[i].tiempoRetorno = tiempoRet;
                        paraMostrar[i].tiempoEspera = tiempoRet - res->ti;
                    }
                }

            } else {
                if (priml->tr == 0 && priml->prox == NULL){
                    /* Aquí termina un proceso y es el ÚLTIMO de todos. */
                    liberarMemoria();
                    fin = true;
                    
                    for (int i = 0; i <= cantProc; i++) {
                        if (res->idProc == paraMostrar[i].idProc) {
                            tiempoRet = tiempoCiclo - res->ta;
                            paraMostrar[i].tiempoRetorno = tiempoRet;
                            paraMostrar[i].tiempoEspera = tiempoRet - res->ti;
                        }
                    }
                }
            }
        }
    }
}

void simulacion(void){

    int saltoTiempo, resTiempo;
    int tecla = 13;
    saltoTiempo = 0;
    resTiempo = tiempoCiclo;
    while (tecla == 13) { 

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
        
        if (priml != NULL) {
            if ((resTiempo + saltoTiempo) == tiempoCiclo) {
                muestrasParciales(priml);
                if (saltoTiempo == 0){
                    saltoTiempo = 1;
                }
                printf("\n");
                printf("Presione:\n");
                printf("-Enter para continuar\n");
                printf("-Backspace para cambiar el salto de tiempo (Actual: %d)\n", saltoTiempo);
                printf("-Escape para salir\n");
                tecla = getch();
                if (tecla == 8) {
                    saltoTiempo = controlDeEntradas("salto de tiempo", 1, 20);
                    tecla = 13;
                } else if (tecla == 27) {
                    break;
                };
                while (tecla != 13 && tecla != 27 && tecla != 8){
                    tecla = getch();
                    if (tecla == 8) {
                        saltoTiempo = controlDeEntradas("salto de tiempo", 1, 20);
                        tecla = 13;
                    } else if (tecla == 27) {
                        break;
                    }
                };   
                resTiempo = tiempoCiclo;
            }
            priml->tr--;
        } else {
            printf("Instante %d: \n", tiempoCiclo);
            printf("No hay procesos en la cola de listos. No hay procesos en ejecución. No hay procesos en memoria.");
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

void reportes(void){
    for (int i = 0; i <= cantProc; i++){
        printf("Proceso %d \n", paraMostrar[i].idProc);
        printf("Tiempo de retorno %d \n", paraMostrar[i].tiempoRetorno);
        printf("Tiempo de espera %d \n", paraMostrar[i].tiempoEspera);
    }
}

int main(void){
    printf("Este programa es un simulador de asignación de memoria y gestión de procesos. Se permiten hasta 10 procesos con un tamaño de 250 como máximo. \n\n");

    menu();

    if (!arch) {
        return(0);
    }

    iniReporte();

    recorridoListaInicial(primp);

    iniciarArreglo();

    simulacion();

    reportes();

    return(0);
};