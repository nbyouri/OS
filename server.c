#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> //Required by mkfifo - 
#include <sys/stat.h> //Required by mkfifo - 
#include <unistd.h> //Required by write - close - read
#include <fcntl.h> //Required by open

#define MSG 256

int main(int argc, char const *argv[])
{
	int inputFifo;
	char fifo_name[] = "server.fifo";
	char message[MSG];

	if(mkfifo(fifo_name, 0777) != 0)
	{
		printf("Was unable to create input fifo\n");
		exit(-1);
	}

	while(1) //Horrible loop
	{
		if((inputFifo = open(fifo_name, O_RDONLY)) == -1)
		{
			printf("Was unable to open the server's fifo\n");
		}
		read(inputFifo, message, MSG);
			printf("%s", message);

		close(inputFifo);
		}

	return 0;

}