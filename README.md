# Keygen-Labs
Trabajo práctico de la cátedra de Sistemas Operativos - FRRe.

**Simulador de Asignación de Memoria y Planificación de Procesos.**

El propósito de esta práctica es la implementación de un simulador que exhiba los aspectos de la Planificación a Corto Plazo y la gestión de la memoria con particiones fijas en un entorno de un solo procesador. Se abordará el ciclo de vida completo de un proceso desde su ingreso al sistema hasta su finalización.

**Consigna:**

Implementar un simulador de asignación de memoria y planificación de procesos con los siguientes requisitos:

- El simulador debe permitir cargar hasta N procesos. Se permite un máximo de 10 procesos, y la asignación de memoria se llevará a cabo con particiones fijas. El esquema de particiones será el siguiente:

  - 100K destinados al Sistema Operativo.
  - 250K para trabajos más grandes.
  - 120K para trabajos medianos.
  - 60K para trabajos pequeños.

- El programa debe permitir el ingreso de nuevos procesos cuando sea posible, manteniendo el grado de multiprogramación en 5. La política de asignación de memoria será Worst-Fit. Para cada proceso, se debe ingresar o leer desde un archivo el Id de proceso, tamaño del proceso, tiempo de arribo y tiempo de irrupción. La planificación de la CPU será dirigida por un algoritmo SJF.

**Salida del Simulador:**

El simulador proporcionará la siguiente información como salida:

- El estado actual del procesador (proceso que se encuentra en ejecución en ese instante).
- La tabla de particiones de memoria, que contendrá (Id de partición, dirección de comienzo de partición, tamaño de la partición, id de proceso asignado a la partición, fragmentación interna).
- El estado de la cola de procesos listos.
- Listado de procesos que no se encuentran en estado de listo ni ejecución, informando el estado en el que se encuentran.

Este simulador ofrece una visión integral de la asignación de memoria y la planificación de procesos en un entorno de un solo procesador, brindando detalles sobre la ejecución, la asignación de memoria y el estado de los procesos en espera.
