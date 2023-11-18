#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

/* Variables globales. */
int userTa, userTi, userTam, acuml = 0;

/* No sé por qué hay que tratar el string del parámetro como un puntero. */
int controlDeEntradas(char *myString, int min, int max) {
    int valor;
    bool bandit;

    do {
        /* Pide al usuario que ingrese un valor entero. */
        printf("\nIngrese el %s\n", myString);
        printf("Se aceptan valores comprendidos entre %d y %d\n:", min, max);

        /* Intenta leer un entero desde la entrada estándar. */
        if (scanf("%d", &valor) != 1) {
            /* La entrada no fue un entero válido. */
            printf("Error: Has ingresado un valor que no es númerico.\n\n");
            bandit = false;
            /* ¡Importante! Limpiar el buffer de entrada para evitar bucles infinitos. */
            while (getchar() != '\n');
        
        } else if (!(valor >= min && valor <= max)) {
            printf("Error: has ingresado un valor numérico fuera del rango permitido.\n\n");
            bandit = false;
        } else {
            bandit = true;
        }
    } while (!bandit);

    return(valor);
}

void cargaManual(int i) {
    printf("\nProceso nro. %d \n", i);
    
    userTa = controlDeEntradas("tiempo de arribo", 0, 500);

    userTi = controlDeEntradas("tiempo de irrupción", 1, 500);
    
    acuml += userTi;

    userTam = controlDeEntradas("tamaño del proceso", 1, 250);
}

int generacionAleatoria(int min, int max){
    srand(time(NULL)); /* Hace que la función rand() genere números distintos con el pasar del tiempo. */
	int numeroAleatorio = min + rand() % max; /* Generamos un número aleatorio entre un mínimo y un máximo solicitado. */
    sleep(1); /* Dejamos que pase un tiempo para que no se repitan los valores. */
    return(numeroAleatorio);
}

void cargaAutomatica(int i) {
    
    userTa = generacionAleatoria(0, 500); 

    userTi = generacionAleatoria(1, 500);
    
    acuml += userTi;

    userTam = generacionAleatoria(1, 250);

}

void menu(void) {
    char respuesta[6];

    do {
        printf("Para cargar manualmente los procesos, ingrese 'manual'. \nSi prefiere generar los procesos de manera automática, ingrese 'auto'. \nSi desea detener la operación, ingrese 'quit'. \n:");
        scanf("%s", respuesta);

        /* Convertir la respuesta a minúsculas para hacer la comparación no sensible a mayúsculas */
        for (int i = 0; i < strlen(respuesta); i++) {
            respuesta[i] = tolower(respuesta[i]);
        }

        /* Si los dos strings son iguales, la función strcmp devuelve 0 */
        if (strcmp(respuesta, "manual") == 0) {
            cargaManual(1);
            break;
        } else if (strcmp(respuesta, "auto") == 0) {
            cargaAutomatica(1);
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

int main(void) {
    menu();

    printf("\nValores cargados: \nTiempo de arribo: %d \nTiempo de irrupción: %d \nAcumulador: %d \nTamaño del proceso: %d \n", userTa, userTi, acuml, userTam);

    return(0);
}