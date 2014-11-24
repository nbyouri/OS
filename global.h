#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/param.h>


#define SUCCESS 0
#define FAIL -1
#define FIFO_EOF 0
#define FIFO_FILE "watchtower.fifo"
#define PILOT_REQUEST "Meteo please?"
#define MSG_SIZE 100
#define RED "\033[22;31m"
#define NOR "\033[00;0m"

struct ATIS{
    size_t length;
    char msg[MSG_SIZE];
};

struct request {
    pid_t pid;
    char msg[MSG_SIZE];
    size_t siz;
};

static bool listen = true;

bool checkyesno(const char *);
void cleanup(int);
void * xmalloc(int, size_t);
void * xrealloc(int, void *, size_t, size_t);
void * clean_ptr(void *);
void fatal(int, const char * restrict, ...);
