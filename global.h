#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>

#define FIFO_FILE "watchtower.fifo"

struct ATIS {
    size_t length;
    char msg[BUFSIZ];
};

void cleanup(int fd);
