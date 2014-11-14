#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> //Required by mkfifo - 
#include <sys/stat.h> //Required by mkfifo - 
#include <unistd.h> //Required by write - 
#include <fcntl.h> //Required by open

#include "global.h"

int main(int argc, char const *argv[])
{
    int serverFifo;

    char message[BUFSIZ] = "You see me rollin \n";

    if((serverFifo = open(FIFO_FILE, O_WRONLY)) == -1) {
        printf("Was unable to open the fifo\n");
    }
    write(serverFifo, message, BUFSIZ);
    close(serverFifo);
    return 0;
}
