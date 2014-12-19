#include <sys/param.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SUCCESS 0
#define FAIL -1
#define FIFO_EOF 0
#define FIFO_FILE "watchtower_listen.fifo"
#define FIFO_FILE_OUT "watchtower_send.fifo"
#define PILOT_REQUEST "Meteo please?"
#define MSG_SIZE 100
#define NBMAXATIS 20
#define RED "\033[22;31m"
#define GRN "\033[22;32m"
#define NOR "\033[00;0m"
#define FICHIERLOCK "lock"
#define FICHIERMETEO "meteo.txt"
#define WAIT_TIME 2
#define WRITE_TIME 1
#define UNREACHABLE "the meteo server is unreachable..."
#define BUSY "the meteo server is busy..."
#define ACK "ACK"
#define NAK "NAK"

/*
 * Maximum we can allocate to a pointer
 * in Linux, taken from BSD Unix.
 */
#ifdef __linux__
#define SIZE_T_MAX (~((size_t) 0))
#endif

/* servers and state */
extern bool listen;
extern int  input;
extern int  output;

/* requests and number of requests */
extern int  nb;
extern char **requests;

int atis(char *);
void createFifos(void);
void openFifos(void);
void operations(void);
bool checkyesno(const char *);
bool exists(const char *);
void delete(const char *);
void cleanup(int);
void * xmalloc(size_t);
void * xrealloc(void *, size_t, size_t);
void cleanPtr(int, char **);
int fatal(const char *, ...) __attribute__((format(printf, 1, 2)));
