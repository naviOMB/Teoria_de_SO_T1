# Tarea 1 - ELO321 (2025)
## Creación y Comunicación de Procesos en C

---

## Descripción del Programa

Este programa, desarrollado en lenguaje **C** utilizando llamadas **POSIX**, permite calcular **números de Armstrong** en paralelo mediante tres procesos hijos.

Cada proceso hijo calcula números de Armstrong de una cantidad de dígitos diferente (`n`, `n-1` y `n-2`) y los guarda en **memoria compartida**.

La memoria compartida se divide en tres bloques independientes para que cada hijo escriba sus resultados sin interferencias.  
Posteriormente, el proceso **padre** espera a que cada hijo termine (usando `waitpid`), lee los resultados desde la memoria y los muestra por pantalla.

El programa requiere como entrada un **número entero entre 3 y 8 (inclusive)**, que indica la cantidad de dígitos con la que comenzará el cálculo.

---

## Integrantes

- **Iván Weber Sanzana** — ROL: 202104092-7  
- **Alicia Pereira Vásquez** — ROL: 202130002-3  

---

## Compilación

Para compilar el programa, se debe utilizar el siguiente comando en una terminal Linux:

```bash
gcc -std=c99 Tarea1_so.c -o tarea1 -lrt -lm
```

---

## Ejecución

Luego de compilar, el programa se ejecuta de la siguiente forma:

```bash
./tarea1 <número de dígitos>
```

Donde `<número de dígitos>` debe estar entre **3 y 8**.

**Ejemplo:**

```bash
./tarea1 5
```

Esto generará los números de Armstrong de **5, 4 y 3 dígitos**, y los desplegará por pantalla.

---

## Ubicación en Aragorn

Los archivos de esta entrega se encuentran disponibles en la siguiente ruta del servidor **Aragorn**:

```
/home/alumnos/ivan.weber/SO
```

Dentro del directorio se encuentra el código fuente y los archivos asociados.

---

*UTFSM - Ingeniería Civil Telemática*
