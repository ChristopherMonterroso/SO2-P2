# Informe técnico final

Universidad de San Carlos de Guatemala

Curso: Sistemas Operativos 2

**Autor:** Christopher Iván Monterroso Alegría
**Carné:** 201902363

---

## Introducción

Este informe detalla el desarrollo e implementación del asignador de memoria denominado **Tamalloc**. Su objetivo es garantizar la inicialización de memoria en cero sin reservar inmediatamente páginas físicas, utilizando un enfoque de lazy-zeroing. Este método busca optimizar el manejo de memoria en sistemas Linux, manteniendo un balance entre memoria reservada y utilizada.

## Configuración del entorno

El entorno de desarrollo se configuró utilizando una máquina virtual basada en Linux, específicamente Linux Mint. Se instalaron las herramientas necesarias para la compilación y modificación del kernel, siguiendo estos pasos:

1. Descargar la imagen del kernel desde [Kernel.org](https://www.kernel.org/pub/linux/kernel/v6.x/linux-6.8.tar.xz).
2. Instalar las dependencias necesarias:

   ```bash
   sudo apt-get install build-essential kernel-package libncurses5-dev fakeroot wget bzip2 openssl
   sudo apt-get install build-essential libncurses-dev bison flex libssl-dev libelf-dev
   ```
3. Descomprimir la imagen y entrar al directorio:

   ```bash
   tar -xf linux-6.8.tar.xz
   cd linux-6.8
   ```
4. Configurar el kernel:

   ```bash
   cp -v /boot/config-$(uname -r) .config
   scripts/config --disable SYSTEM_TRUSTED_KEYS
   scripts/config --disable SYSTEM_REVOCATION_KEYS
   ```
5. Editar el archivo `Makefile` para personalizar el kernel:

   ```bash
   nano Makefile
   ```

   Modificar la línea:

   ```makefile
   EXTRAVERSION = -49-chris
   ```
6. Compilar el kernel:

   ```bash
   sudo -s
   make oldconfig
   make localmodconfig
   make -j$(nproc --ignore=1)
   make modules_install
   make install
   make headers_install
   update-grub2
   ```
7. Reiniciar el sistema para utilizar el kernel personalizado.

## Diseño del Algoritmo

### Personalización del nombre del sistema

El algoritmo **Tamalloc** sigue estos pasos:

* Reserva de memoria virtual sin asignar páginas físicas usando **mmap()** con las banderas `<span>MAP_NORESERVE</span>`, `<span>MAP_ANONYMOUS</span>`, y `<span>MAP_PRIVATE</span>`.
* Implementación de un manejador de fallos de página (page fault handler) que inicializa en cero la página requerida al ser accedida.
* Recolecta estadísticas mediante syscalls específicas, proporcionando información en tiempo real sobre memoria reservada y utilizada.

### Implementación

* **Asignación de Memoria**: Utiliza mmap() para crear un espacio reservado en memoria virtual sin asignar físicamente páginas.
* **Lazy-Zeroing**: El controlador de fallos de página intercepta el primer acceso a cada página y la inicializa en cero.
* **Estadísticas de Uso**: Dos syscalls personalizadas permiten recopilar y mostrar estadísticas sobre el uso de memoria a nivel de proceso y sistema.



## Resultados y análisis

Pruebas realizadas con scripts  demostraron que la memoria reservada no se asigna físicamente hasta que es accedida. Esto se reflejó en la reducción del uso de memoria RSS inicial, lo que permite un mejor manejo del over-commit.

**Proceso Individual:**

* Memoria reservada: 500 MB
* Memoria utilizada: 150 MB (30%)
* OOM Score: 5

**Sistema Total:**

* Memoria reservada: 4 GB
* Memoria utilizada: 2 GB

## Problemas encontrados


| Problema                                 | Causa                                                               | Solución                                                                                                 |
| ---------------------------------------- | ------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------- |
| Fallas en la integración conel kernel  | Incompatibilidad con estructuras de<br /> memoria<br /> existentes  | Ajustes en el manejo de mmap y control de fallos de página                                               |
| Estadísticas inconsistentes en syscalls | Errores en la recopilación de datos <br />al nivel<br />de proceso | Revisión del acceso a estructuras internas del kernel y <br />corrección<br />de las llamadas a syscall |


## Crónograma



| Día  | Actividad                                                                               |
| ----- | --------------------------------------------------------------------------------------- |
| 20/12 | Investigación sobre algoritmos de asignación de memoria y configuración del entorno. |
| 21/12 | Implementación inicial del algoritmo Tamalloc con lazy-zeroing y pruebas básicas.     |
| 22/12 | Desarrollo e integración de syscalls para estadísticas y optimización del algoritmo. |
| 25/12 | Pruebas avanzadas, resolución de errores y elaboración del informe técnico.          |

## Reflexión Personal

Este proyecto me permitió mejorar mi comprensión sobre los asignadores de memoria y los manejadores de fallos de página. Aunque encontré dificultades técnicas, como errores en la recopilación de estadísticas y la integración con el kernel.

En conclusión, **Tamalloc** representa una herramienta eficiente para optimizar la memoria en sistemas Linux.
