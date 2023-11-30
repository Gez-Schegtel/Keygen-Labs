#include <stdio.h> // For ´printf´ function
#include <string.h> // For ´strlen´ function
#include <ctype.h> // For ´tolower´ function
#include <stdlib.h> // For ´exit´ function

#define MAX_INPUT_LENGTH 6  // Set your desired maximum input length

void menu(void) {
    char buffer[MAX_INPUT_LENGTH + 2];  // Adding 2 for the newline character and null terminator

    while(1) {

        printf("Para cargar manualmente los procesos, ingrese 'manual'.\n");
        printf("Para leer los procesos desde el archivo 'procesos.csv', ingrese 'leer'.\n");
        printf("Para generar de manera automática los procesos, ingrese 'auto'.\n");
        printf("Si desea detener la operación, ingrese 'salir'.\n>> ");

        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            // Remove the newline character from the end of the string, if present
            if (buffer[strlen(buffer) - 1] == '\n') {
                buffer[strlen(buffer) - 1] = '\0';

                // We make the input case insensitive
                for (int i = 0; buffer[i] != '\0'; i++) {
                    buffer[i] = tolower(buffer[i]);
                }

            } else {
                // Handle the case where the input exceeds the maximum length
                printf("\nLa entrada excedió los límites. La longitud máxima es de %d caracteres.\n\n", MAX_INPUT_LENGTH);
                while(getchar() != '\n'); // We need to clean up the input to avoid re-execute the loop multiple times
                continue; // We can ask for user input again
            }

            // Check if the input is one of the specified characters
            char inputChar;
            if (sscanf(buffer, " %c", &inputChar) == 1) {
                
                if (strcmp(buffer, "manual") == 0) {
                    printf("Se seleccionó la carga manual de procesos.\n");
                    break;
                } else {
                    if (strcmp(buffer, "leer") == 0) {
                        printf("Se seleccionó la carga de procesos desde un archivo.\n");
                        break;
                    } else {
                        if (strcmp(buffer, "auto") == 0) {
                            printf("Se ha seleccionado la carga automática de procesos.\n");
                            break;
                        } else {
                            if (strcmp(buffer, "salir") == 0) {
                                printf("Deteniendo la ejecución del programa...\n");
                                exit(0);
                            } else {
                                printf("\nHas ingresado una opción incorrecta.\n\n");
                                continue;
                            }
                        }
                    }
                }

            } else {
                // Here we verify that the input wasn't something like TAB or another "special" keys
                printf("\nHas ingresado un valor incorrecto.\n");
                continue;
            }
        } else {
            printf("Error reading input.\n");
            exit(1);
        }
    }
}

int main(void) {
    menu();
    return(0);
}
