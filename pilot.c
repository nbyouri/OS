#include "global.h"

int main(void) {
    int server = -1;
    int out_server = -1;

    struct request req;

    req.pid = getpid();
    memcpy(req.msg, PILOT_REQUEST, MSG_SIZE);
    req.siz = strlen(req.msg);

    if((server = open(FIFO_FILE, O_WRONLY)) == FAIL) {

        fatal(server, "Couldn't open fifo %s\n", FIFO_FILE);

    } else {

        printf("Sending REQUEST...\n");

        if (write(server, &req, sizeof(req)) == FAIL) {

            fatal(server, "Failed to write message\n");

        } else {
            char response[MSG_SIZE];

            if ((out_server = open(FIFO_FILE_OUT, O_RDONLY)) == FAIL) {
                fatal(out_server, "Couldn't open output file...\n");
            } else {
                if (read(out_server, response, MSG_SIZE) == FAIL) {
                    fatal(out_server, "Failed to read response from output fifo\n");
                } else {
                    printf("Got response ! => %s\n", response);
                }
            }

            // close the fifo
            if (close(server) == FAIL) {

                fatal(server, "Failed to close the fifo\n");

            }
        }

        return EXIT_SUCCESS;
    }
}

