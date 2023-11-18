#include <stdio.h>

int main() {
    int valor;

    // Pide al usuario que ingrese un valor entero
    printf("Ingrese un valor entero: ");

    // Intenta leer un entero desde la entrada estándar
    if (scanf("%d", &valor) == 1) {
        // La entrada fue un entero válido
        printf("Has ingresado: %d\n", valor);
    } else {
        // La entrada no fue un entero válido
        printf("Error: Debes ingresar un valor entero.\n");
    }

    return 0;
}