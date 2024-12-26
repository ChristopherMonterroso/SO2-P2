#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define SYSCALL_TAMALLOC 551

int main(int argc, char *argv[]) {
    size_t size = 4096; // Valor predeterminado: 4 KB

    // Si se proporciona un argumento, intenta convertirlo a tamaño
    if (argc > 1) {
        size = strtoul(argv[1], NULL, 10); // Convierte el argumento a número
        if (size == 0) {
            fprintf(stderr, "Error: el tamaño debe ser mayor a 0.\n");
            return EINVAL;
        }
    }

    // Llama a la syscall tamalloc con el tamaño especificado
    void *addr = (void *)syscall(SYSCALL_TAMALLOC, size);

    if ((long)addr < 0) {
        perror("tamalloc");
        return errno;
    }

    printf("Memory allocated at: %p, Size: %zu bytes\n", addr, size);
    return 0;
}

