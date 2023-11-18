#include <stdio.h>
#include <stdbool.h>

int main(void) {
    int valor;
    bool bandit;

    do {
        /* Pide al usuario que ingrese un valor entero. */
        printf("Ingrese un valor entero comprendido entre el 0 y el 100: ");

        /* Intenta leer un entero desde la entrada estándar. */
        if (scanf("%d", &valor) != 1) {
            /* La entrada no fue un entero válido. */
            printf("Error: Has ingresado un valor que no es númerico.\n");
            bandit = false;
            /* ¡Importante! Limpiar el buffer de entrada para evitar bucles infinitos. */
            while (getchar() != '\n');
        
        } else if (!(valor >= 0 && valor <= 100)) {
            printf("Error: has ingresado un valor numérico fuera del rango permitido.\n");
            bandit = false;
        } else {
            bandit = true;
        }
    } while (!bandit);

    return(0);
}
