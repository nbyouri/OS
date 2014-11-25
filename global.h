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
#define FIFO_FILE "watchtower_listen.fifo"
#define FIFO_FILE_OUT "watchtower_send.fifo"
#define PILOT_REQUEST "Meteo please?"
#define MSG_SIZE 100
#define RED "\033[22;31m"
#define NOR "\033[00;0m"

struct ATIS{
    size_t siz;			// 8 bytes 
    char msg[MSG_SIZE]; // 100 bytes
};

struct request {
    pid_t pid;			// 4 bytes
    char msg[MSG_SIZE]; // 100 bytes
    size_t siz;			// 8 bytes
};

static bool listen = true;
static int  input  = -1;
static int  output = -1;

int gen_atis(void);
void atis(char *, struct request);
bool checkyesno(const char *);
void cleanup(int);
void * xmalloc(size_t);
void * xrealloc(void *, size_t, size_t);
void * clean_ptr(void *);
void fatal(const char * restrict, ...);
