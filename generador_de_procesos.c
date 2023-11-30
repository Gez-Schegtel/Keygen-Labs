#include <stdio.h>
#include <stdbool.h>

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

int main() {
    FILE *archivo;
    char nombreArchivo[] = "procesos.csv"; // Nombre del archivo CSV
    int numRegistros; // Variable para almacenar la cantidad de registros a generar
    int id = 1; // Inicializar el ID en 1
    int ta; 
    int ti; 
    int tam;

    // Solicitar al usuario la cantidad de registros a generar
    printf("Ingrese la cantidad de registros a generar (1-10): ");
    scanf("%d", &numRegistros);

    // Verificar que la cantidad ingresada esté dentro del rango permitido
    if (numRegistros < 1 || numRegistros > 10) {
        printf("Ingrese un número válido entre 1 y 10.\n");
        return 1;
    }

    // Abrir el archivo en modo escritura, si no existe se creará, si existe se sobrescribirá
    archivo = fopen(nombreArchivo, "w");

    if (archivo == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return 1;
    }

    // Escribir encabezados en el archivo CSV
    fprintf(archivo, "idProc,ta,ti,tam\n");

    while (id <= numRegistros) {
        printf("\nProceso nro. %d \n", id);
        // Solicitar datos al usuario
        ta = controlDeEntradas("tiempo de arribo", 0, 20);
        ti = controlDeEntradas("tiempo de irrupción", 1, 20);
        tam = controlDeEntradas("tamaño del proceso", 1, 250);

        // Escribir los datos en el archivo CSV con el ID actual
        fprintf(archivo, "%d,%d,%d,%d\n", id, ta, ti, tam);
        id++; // Incrementar el ID
    }

    // Cerrar el archivo
    fclose(archivo);

    printf("Se han generado y escrito %d registros en %s.\n", numRegistros, nombreArchivo);

    return 0;
}