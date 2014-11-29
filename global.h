
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <time.h>
#include <poll.h>
#include <sys/stat.h>
#include <sys/param.h>


#define SUCCESS 0
#define FAIL -1
#define FIFO_EOF 0
#define FIFO_FILE "watchtower_listen.fifo"
#define FIFO_FILE_OUT "watchtower_send.fifo"
#define PILOT_REQUEST "Meteo please?"
#define MSG_SIZE 100
#define NBMAXATIS 20
#define RED "\033[22;31m"
#define NOR "\033[00;0m"
#define FICHIERLOCK "lock"
#define FICHIERMETEO "meteo.txt"

#ifdef __linux__
#define SIZE_T_MAX (~((size_t) 0))
#endif

struct ATIS{
    size_t siz;			// 8 bytes
    char msg[MSG_SIZE]; // 100 bytes
};

struct request {
    pid_t pid;			// 4 bytes
    char msg[MSG_SIZE]; // 100 bytes
    size_t siz;			// 8 bytes
};

extern bool listen;
extern int  input;
extern int  output;

void genAtis(void);
int atis(char *);
void createFifos(void);
void openFifos(void);
void operations(void);
bool checkyesno(const char *);
int delete(const char *);
void cleanup(int);
void * xmalloc(size_t);
void * xrealloc(void *, size_t, size_t);
void * cleanPtr(void *);
int fatal(const char * restrict, ...);
