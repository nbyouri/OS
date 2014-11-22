#include "global.h"

static bool listen = true;

void cleanup(int fd) {
    struct stat info;

    printf("\ncleaning up...\n");

    // close the fifo file
    if (close(fd) == FAIL) {
        printf("Couldn't close fifo.\n");
    }

    // delete the fifo file if it exists
    if (unlink(FIFO_FILE) < 0) {
        if (stat(FIFO_FILE, &info) == FAIL) {
            printf("The fifo file doesn't exist\n");
        } else {
            printf("Couldn't delete fifo\n");
        }
        exit(EXIT_FAILURE);
    }

    // exit nicely
    exit(EXIT_SUCCESS);

}

int main(void) {
    // fifo file descriptor number
    int input = 0;
    int result = 0;
    // message to read
    fd_set      readset;
    struct timeval   tv;
    char msg[80];

    // setup signal, so if programs exits 
    // abruptly, the fifo still gets cleanup up
    sigset(SIGINT, &cleanup);

    // create the fifo
    if (mkfifo(FIFO_FILE, S_IRUSR | S_IWUSR) == FAIL) {
        printf("Was unable to create input fifo\n");
        return FAIL;
    }

    // open the fifo
    if ((input = open(FIFO_FILE, O_RDONLY)) == FAIL) {
        printf("Was unable to open the server's fifo\n");
        cleanup(input);
        return FAIL;
    }

    tv.tv_sec = 1;
    tv.tv_usec = 0;

    while (listen) {
        FD_ZERO(&readset);
        FD_SET(input, &readset);
        result = select (input+1, &readset, NULL, NULL, &tv);

        if (result > 0) {
            // read and write the message
            if (read(input, msg, strlen(msg)) == FAIL) {
                printf("Was unable to read the message\n");
                cleanup(input);
            } else {
                if (strnstr(msg, PILOT_REQUEST, MSG_SIZE) == NULL) {
                    write(STDOUT_FILENO, msg, strlen(msg));
                } else {
                    printf("Meaningless message intercepted, skipping...\n");
                }
            }
        }
    }

    // exit gracefully
    cleanup(input);
}
