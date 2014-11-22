#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>

#define SUCCESS 0
#define FAIL -1
#define FIFO_FILE "watchtower.fifo"
#define PILOT_REQUEST "Meteo please?"
#define MSG_SIZE 100

struct ATIS {
    size_t length;
    char msg[MSG_SIZE];
};

struct request {
    pid_t pid;
    char msg[MSG_SIZE];
    size_t siz;
};

void cleanup(int fd);
