#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <errno.h>

#define SYSCALL_MEMORY_USAGE_GENERAL 553

struct global_memory_stats {
    unsigned long total_reserved_mb;
    unsigned long total_committed_mb;
};

int main() {
    struct global_memory_stats stats;

    if (syscall(SYSCALL_MEMORY_USAGE_GENERAL, &stats) != 0) {
        perror("memory_usage_general");
        return errno;
    }

    printf("Total Reserved: %lu MB\n", stats.total_reserved_mb);
    printf("Total Committed: %lu MB\n", stats.total_committed_mb);
    return 0;
}

