#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h> // Para atoi

#define SYSCALL_MEMORY_USAGE_TABLE 552

struct memory_stats {
    unsigned long reserved_kb;
    unsigned long committed_kb;
    int oom_score;
};

int main(int argc, char *argv[]) {
    struct memory_stats stats;
    pid_t pid = 0; // Valor predeterminado: el proceso actual

    // Verifica si se proporcionó un argumento
    if (argc > 1) {
        pid = atoi(argv[1]); // Convierte el argumento a entero
        if (pid <= 0) {
            fprintf(stderr, "Error: el PID debe ser un número entero positivo.\n");
            return EINVAL;
        }
    }

    // Llama a la syscall con el PID especificado
    if (syscall(SYSCALL_MEMORY_USAGE_TABLE, pid, &stats) != 0) {
        perror("memory_usage_table");
        return errno;
    }

    // Muestra las estadísticas de memoria
    printf("PID: %d\n", pid);
    printf("Reserved: %lu KB\n", stats.reserved_kb);
    printf("Committed: %lu KB\n", stats.committed_kb);
    printf("OOM Score: %d\n", stats.oom_score);

    return 0;
}

