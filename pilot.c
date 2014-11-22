#include "global.h"

int main(void) {
    int server;

    struct request * req1 = NULL;
    req1 = malloc(sizeof(*req1));

    req1->pid = getpid();
    strncpy(req1->msg, PILOT_REQUEST, strnlen(PILOT_REQUEST, BUFSIZ));
    req1->siz = strlen(req1->msg);

    char msg[MSG_SIZE];
    sprintf(msg, "REQ: %d :: %s (size=%zu)\n", req1->pid, req1->msg, req1->siz);

    if((server = open(FIFO_FILE, O_WRONLY)) == FAIL) {
        printf("Was unable to open the fifo\n");
    } else {
        // write the structure
        printf("Sending REQUEST...\n");
        if (write(server, msg, strlen(msg)) == FAIL) {
            printf("Failed to write pid\n");
        }

        // close the fifo
        if (close(server) == FAIL) {
            printf("Failed to close the fifo\n");
        }
    }

    free(req1);
    req1 = NULL;
    return EXIT_SUCCESS;
}
