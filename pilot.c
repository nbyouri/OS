#include "global.h"

void pilot_cleanup(int, int, int);

int main(void) {
    int server = -1;
    int out_server = -1;

    char request[MSG_SIZE];
    char response[MSG_SIZE];

    memcpy(request, PILOT_REQUEST, MSG_SIZE);

    if((server = open(FIFO_FILE, O_WRONLY)) == FAIL) {

        printf("Couldn't open fifo %s\n", FIFO_FILE);

    } else {

        printf("Sending REQUEST...\n");

        if (write(server, request, sizeof(request)) == FAIL) {

            printf("Failed to write message\n");
            pilot_cleanup(server, out_server, FAIL);

        } else {

            if ((out_server = open(FIFO_FILE_OUT, O_RDONLY)) == FAIL) {

                printf("Couldn't open output file...\n");
                pilot_cleanup(server, out_server, FAIL);

            } else {

                if (read(out_server, response, MSG_SIZE) == FAIL) {

                    printf("Failed to read response from output fifo\n");
                    pilot_cleanup(server, out_server, FAIL);

                } else {

                    printf("Got response ! => %s\n", response);

                }
            }
        }
        pilot_cleanup(server, out_server, EXIT_SUCCESS);
    }
}


void pilot_cleanup(int in_serv, int out_serv, int status){
    // close the file descriptors
    if (in_serv != FAIL) {
        if (close(in_serv) == FAIL) {
            printf("Couldn't close input file descriptor %d\n", in_serv);
            exit(status);
        }
    }
    if (out_serv != FAIL) {
        if (close(out_serv) == FAIL) {
            printf("Couldn't close output file descriptor %d\n", out_serv);
            exit(status);
        }
    }
    exit(EXIT_SUCCESS);
}

