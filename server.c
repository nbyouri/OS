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
    exit(EXIT_SUCCESS);
}

int main(int argc, char const **argv)
{
    // fifo file descriptor number
    int input = 0;
    // message to read
    char message[BUFSIZ];

    // setup signal, so if programs exits 
    // abruptly, the fifo still gets cleanup up
    sigset(SIGINT, &cleanup);

    // create the fifo
    if (mkfifo(FIFO_FILE, 0777) == -1) {
        printf("Was unable to create input fifo\n");
        return -1;
    }

    // open the fifo
    if ((input = open(FIFO_FILE, O_RDONLY)) == -1) {
        printf("Was unable to open the server's fifo\n");
        cleanup(input);
        return -1;
    }

    // read and write the message
    if (read(input, message, BUFSIZ) == -1) {
        printf("Was unable to read the message\n");
        cleanup(input);
    } else {
        if (write(STDOUT_FILENO, message, BUFSIZ) == -1) {
            printf("Was unable to write the message\n");
            cleanup(input);
            return -1;
        }
    }

    // exit gracefully
    cleanup(input);
}
