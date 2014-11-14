#include "global.h"

int main(int argc, char const **argv)
{
    int server;

    if((server = open(FIFO_FILE, O_WRONLY)) == -1) {
        printf("Was unable to open the fifo\n");
    }

    // write request message
    if (write(server, PILOT_REQUEST, BUFSIZ) == -1) {
        printf("Failed to write message\n");
    }

    if (close(server) == -1) {
        printf("Failed to close the fifo\n");
    }

    return 0;
}
