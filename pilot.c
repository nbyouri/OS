#include "global.h"

int main(void) {
    int server;

    struct request * req1 = NULL;
    req1 = malloc(sizeof(*req1));

    req1->pid = getpid();
    strncpy(req1->msg, PILOT_REQUEST, strnlen(PILOT_REQUEST, BUFSIZ));
    req1->siz = (ssize_t)strlen(req1->msg);

    char msg[MSG_SIZE];
    sprintf(msg, "REQ: %d :: %s (size=%zu)\n", req1->pid, req1->msg, req1->siz);

    if((server = open(FIFO_FILE, O_WRONLY)) == FAIL) {
        fatal(server, "Was unable to open the fifo\n");
    } else {

        printf("Sending REQUEST...\n");

        write(server, &req1->siz, sizeof(req1->siz));

        if (write(server, msg, strlen(msg)) == FAIL) {
            fatal(server, "Failed to write pid\n");
        }

        // close the fifo
        if (close(server) == FAIL) {
            fatal(server, "Failed to close the fifo\n");
        }
    }
    
    req1 = clean_ptr(req1);

    return EXIT_SUCCESS;
}

