#include "global.h"

int main(void) {
    int server = -1;

    struct request * req1 = NULL;
    req1 = xmalloc(server, sizeof(*req1));

    req1->pid = getpid();
    strncpy(req1->msg, PILOT_REQUEST, strnlen(PILOT_REQUEST, BUFSIZ));
    req1->siz = (ssize_t)strlen(req1->msg);

    char msg[MSG_SIZE];
    sprintf(msg, "REQ: %d :: %s (size=%zu)\n", req1->pid, req1->msg, req1->siz);

    if((server = open(FIFO_FILE, O_WRONLY)) == FAIL) {

        fatal(server, "Couldn't open fifo %s\n", FIFO_FILE);

    } else {

        printf("Sending REQUEST...\n");

        write(server, &req1->siz, sizeof(req1->siz));

        if (write(server, msg, strlen(msg)) == FAIL) {

            fatal(server, "Failed to write message\n");

        }

        // close the fifo
        if (close(server) == FAIL) {

            fatal(server, "Failed to close the fifo\n");
            
        }
    }
    
    req1 = clean_ptr(req1);

    return EXIT_SUCCESS;
}

