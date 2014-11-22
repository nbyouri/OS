#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>

/*
 *
 * TODO : XMALLOC, ERROR FUNCTION
 *
 */
#define SUCCESS 0
#define FAIL -1
#define FIFO_EOF 0
#define FIFO_FILE "watchtower.fifo"
#define PILOT_REQUEST "Meteo please?"
#define MSG_SIZE 100

struct ATIS {
    ssize_t length;
    char msg[MSG_SIZE];
};

struct request {
    pid_t pid;
    char msg[MSG_SIZE];
    ssize_t siz;
};

bool checkyesno(const char *);
void cleanup(int);

