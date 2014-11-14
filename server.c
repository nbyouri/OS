#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> //Required by mkfifo - 
#include <sys/stat.h> //Required by mkfifo - 
#include <unistd.h> //Required by write - close - read
#include <fcntl.h> //Required by open
#include <signal.h>

#define MSG 256
#define FIFO "server.fifo"

void cleanup(int inputFifo) {
    struct stat info;

    // close the fifo file
    if (close(inputFifo) == -1) {
        printf("Couldn't close fifo.\n");
    }

    // delete the fifo file if it exists
    if (unlink(FIFO) < 0) {
        if (stat(FIFO, &info) == -1) {
            printf("The fifo file doesn't exist\n");
        } else {
            printf("Couldn't delete fifo\n");
        }
        exit(EXIT_FAILURE);
    }

    // exit anyway
    exit(EXIT_SUCCESS);
}

int main(int argc, char const *argv[])
{
    int inputFifo;
    char message[MSG];
    sigset(SIGINT, &cleanup);

    if(mkfifo(FIFO, 0777) != 0) {
        printf("Was unable to create input fifo\n");
        return -1;
    }

    if ((inputFifo = open(FIFO, O_RDONLY)) == -1) {
        printf("Was unable to open the server's fifo\n");
        return -1;
    }

    while(1) {
        read(inputFifo, message, MSG);
        printf("%s", message);
    }

    return 0;
}
