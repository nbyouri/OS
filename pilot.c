#include "global.h"

int main(int argc, char const **argv)
{
    int server;

    char message[BUFSIZ] = "You see me rollin \n";

    if((server = open(FIFO_FILE, O_WRONLY)) == -1) {
        printf("Was unable to open the fifo\n");
    }

    if (write(server, message, BUFSIZ) == -1) {
        printf("Failed to write message\n");
    }

    if (close(server) == -1) {
        printf("Failed to close the fifo\n");
    }

    return 0;
}
