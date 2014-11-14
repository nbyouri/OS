#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> //Required by mkfifo - 
#include <sys/stat.h> //Required by mkfifo - 
#include <unistd.h> //Required by write - 
#include <fcntl.h> //Required by open

#define MSG 256
#define FIFO "server.fifo"

int main(int argc, char const *argv[])
{
    int serverFifo;

    char message[MSG] = "You see me rollin \n"; //test message

    if((serverFifo = open(FIFO, O_WRONLY)) == -1) {
        printf("Was unable to open the fifo\n");
    }
    write(serverFifo, message, MSG);
    close(serverFifo);
    return 0;
}
