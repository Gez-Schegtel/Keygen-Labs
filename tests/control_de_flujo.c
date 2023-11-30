#include <stdio.h> // For ´printf´ function
#include <string.h> // For ´strlen´ function
#include <ctype.h> // For ´tolower´ function
#include <stdlib.h> // For ´exit´ function

#define MAX_INPUT_LENGTH 1  // Set your desired maximum input length

char menu(void) {
    char buffer[MAX_INPUT_LENGTH + 2];  // Adding 2 for the newline character and null terminator

    while(1) {

        printf("Presione la tecla C para continuar. \n");
        printf("Presione la tecla T para dar un salto de tiempo. \n");
        printf("Presione la tecla F para para ir hasta el final. \n");
        printf("Presione la tecla S para salir. \n>> ");

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
                printf("Input too long. Maximum length is %d characters.\n", MAX_INPUT_LENGTH);
                while(getchar() != '\n'); // We need to clean up the input to avoid re-execute the loop multiple times
                continue; // We can ask for user input again
            }

            // Check if the input is one of the specified characters
            char inputChar;
            if (sscanf(buffer, " %c", &inputChar) == 1) {
                
                if (inputChar == 'c'){
                    printf("\nContinuamos la ejecución al tiempo de ciclo siguiente.\n");
                    return('c');
                } else {
                    if (inputChar == 't'){
                        printf("\nHaremos un salto de tiempo.\n");
                        printf("\n");
                        return('t');
                    } else {
                        if (inputChar == 'f'){
                            printf("Nos desplazamos hasta el último tiempo de ciclo.\n");
                            return('f');
                        } else {
                            if (inputChar == 's'){
                                printf("\nDeteniendo la ejecución del programa.\n");
                                exit(1);
                            } else {
                                printf("\nHas ingresado un valor incorrecto.\n");
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
    printf("\nEl caracter de la función es >> %c\n", menu());
    return(0);
}
