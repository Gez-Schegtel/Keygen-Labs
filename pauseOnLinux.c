#include <stdio.h>

void pausarPrograma() {
    printf("Presiona Enter para continuar...");
    while (getchar() != '\n');
}

int main() {
    // Tu código aquí

    // Llama a la función para pausar
    pausarPrograma();

    return 0;
}
