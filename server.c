#include "global.h"

void cleanup(int fd) {
    struct stat info;

    // close the fifo file
    if (close(fd) == -1) {
        printf("Couldn't close fifo.\n");
    }

    // delete the fifo file if it exists
    if (unlink(FIFO_FILE) < 0) {
        if (stat(FIFO_FILE, &info) == -1) {
            printf("The fifo file doesn't exist\n");
        } else {
            printf("Couldn't delete fifo\n");
        }
        exit(EXIT_FAILURE);
    }

    // exit anyway
    exit(EXIT_SUCCESS);
}

int main(int argc, char const **argv)
{
    int inputFifo;
    char message[BUFSIZ];
    sigset(SIGINT, &cleanup);

    if(mkfifo(FIFO_FILE, 0777) != 0) {
        printf("Was unable to create input fifo\n");
        return -1;
    }

    if ((inputFifo = open(FIFO_FILE, O_RDONLY)) == -1) {
        printf("Was unable to open the server's fifo\n");
        return -1;
    }

    // write the message
    read(inputFifo, message, BUFSIZ);
    write(0, message, BUFSIZ);

    return EXIT_SUCCESS;
}
