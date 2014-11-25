#include "global.h"

int main(void) {
    int server = -1;
    int out_server = -1;

    struct request req;

    req.pid = getpid();
    memcpy(req.msg, PILOT_REQUEST, MSG_SIZE);
    req.siz = strlen(req.msg);

    if((server = open(FIFO_FILE, O_WRONLY)) == FAIL) {

        fatal("Couldn't open fifo %s\n", FIFO_FILE);

    } else {

        printf("Sending REQUEST...\n");

        if (write(server, &req, sizeof(req)) == FAIL) {

            fatal("Failed to write message\n");

        } else {
            char response[MSG_SIZE];

            if ((out_server = open(FIFO_FILE_OUT, O_RDONLY)) == FAIL) {
                fatal("Couldn't open output file...\n");
            } else {
                if (read(out_server, response, MSG_SIZE) == FAIL) {
                    fatal("Failed to read response from output fifo\n");
                } else {
                    printf("Got response ! => %s\n", response);
                }
            }

            // close the fifo
            if (close(server) == FAIL) {

                fatal("Failed to close the fifo\n");

            }

            if (close(out_server) == FAIL) {

                fatal("Failed to close the output fifo\n");
                
            }
        }

        return EXIT_SUCCESS;
    }
}

