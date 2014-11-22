#include "global.h"

static bool listen = true;

bool checkyesno(const char *msg) {
    char        input[BUFSIZ];
    char        res;

    do {

        printf("%s %s", msg, " ? ([o]ui/[n]on) : ");
        fgets(input, sizeof(input), stdin);
        sscanf(input, "%c", &res);

    } while (res != 'n' && res != 'o');

    if (res == 'n') {
        listen = false;
    }
    
    return listen;
}

void cleanup(int fd) {
    struct stat         info;

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
    int                 input;

    // message to read
    fd_set              readset;
    struct timeval      tv;
    ssize_t             size;
    char                msg[MSG_SIZE];

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

    // refresh the loop every second
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    while (listen) {
        // zero readset and set it to input
        FD_ZERO(&readset);
        FD_SET(input, &readset);
        if (select (input+1, &readset, NULL, NULL, &tv) > 0) {

            // read and write the message
            if (read(input, &size, sizeof(ssize_t)) == FAIL) {

                printf("Nothing to read...\n");
                cleanup(input);
                return FAIL;
            } else {
                int packet = (int)read(input, msg, MSG_SIZE);

                if (packet == FAIL) {

                    printf("couldn't read message\n");
                    // all data has been received
                } else if (packet == FIFO_EOF) {

                    listen = checkyesno("Keep listening");
                    // message actions
                } else {

                    if (strnstr(msg, PILOT_REQUEST, MSG_SIZE) != NULL)  {
                        write(STDOUT_FILENO, msg, strlen(msg));
                    }
                }
            }
        }
    }

    // exit gracefully
    cleanup(input);
}

