#include "global.h"

int main(void) {
    int server = -1;

    struct request req;

    req.pid = getpid();
    //strncpy(req.msg, PILOT_REQUEST, strlen(PILOT_REQUEST));
    memcpy(req.msg, PILOT_REQUEST, MSG_SIZE);
    req.siz = strlen(req.msg);

    printf("REQ :: %d => %s (%zu)\n", req.pid, req.msg, req.siz);

    //char msg[MSG_SIZE];
    //sprintf(msg, "REQ: %d :: %s (size=%zu)\n", req1->pid, req1->msg, req1->siz);

    if((server = open(FIFO_FILE, O_WRONLY)) == FAIL) {

        fatal(server, "Couldn't open fifo %s\n", FIFO_FILE);

    } else {

        printf("Sending REQUEST...\n");

        if (write(server, &req, sizeof(req)) == FAIL) {

            fatal(server, "Failed to write message\n");

        }

        // close the fifo
        if (close(server) == FAIL) {

            fatal(server, "Failed to close the fifo\n");
            
        }
    }

    return EXIT_SUCCESS;
}

