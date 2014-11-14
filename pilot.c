#include "global.h"

int main(int argc, char const **argv)
{
    int server;

    struct request req1;
    snprintf(req1.pid, sizeof(req1.pid), "%d", getpid());
    strncpy(req1.msg, PILOT_REQUEST, strlen(PILOT_REQUEST));

    if((server = open(FIFO_FILE, O_WRONLY)) == -1) {
        printf("Was unable to open the fifo\n");
    }

    // write request message
    if (write(server, &req1, BUFSIZ) == -1) {
        printf("Failed to write message\n");
    }

    if (close(server) == -1) {
        printf("Failed to close the fifo\n");
    }

    return 0;
}
