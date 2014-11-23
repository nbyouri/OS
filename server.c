#include "global.h"

int main(void) {
    // fifo file descriptor number
    int                 input = -1;

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

        fatal(input, "Was unable to create input fifo\n");
        
    }

    // open the fifo
    if ((input = open(FIFO_FILE, O_RDONLY)) == FAIL) {

        fatal(input, "Was unable to open the server's fifo\n");

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

                fatal(input, "Nothing to read...\n");

            } else {

                int packet = (int)read(input, msg, MSG_SIZE);

                if (packet == FAIL) {

                    printf("couldn't read message\n");

                } else if (packet == FIFO_EOF) {

                    listen = checkyesno("Keep listening");

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

