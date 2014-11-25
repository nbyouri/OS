#include "global.h"

// TODO: add local cleanup

int main(void) {
    int server = -1;
    int out_server = -1;

    struct request req;
    char response[MSG_SIZE];

    req.pid = getpid();
    memcpy(req.msg, PILOT_REQUEST, MSG_SIZE);
    req.siz = strlen(req.msg);

    if((server = open(FIFO_FILE, O_WRONLY)) == FAIL) {

        printf("Couldn't open fifo %s\n", FIFO_FILE);

    } else {

        printf("Successfully opened %d\n", server);
        printf("Sending REQUEST...\n");

        if (write(server, &req, sizeof(req)) == FAIL) {

            printf("Failed to write message\n");

        } else {

            if ((out_server = open(FIFO_FILE_OUT, O_RDONLY)) == FAIL) {

                printf("Couldn't open output file...\n");

            } else {

                printf("Successfully opened %d\n", out_server);

                if (read(out_server, response, MSG_SIZE) == FAIL) {

                    printf("Failed to read response from output fifo\n");

                } else {

                    printf("Got response ! => %s\n", response);

                }
            }

            // close the fifo
            if (close(server) == FAIL) {

                printf("Failed to close the fifo\n");

            } else {

                printf("Successfully closed fd %d\n", server);

            }

            if (close(out_server) == FAIL) {

                printf("Failed to close the output fifo\n");
                
            } else {

                printf("Successfully closed fd %d\n", out_server);

            }
        }

        return EXIT_SUCCESS;
    }
}

