#include "global.h"

#define MAX_TRY 20

int input = -1;
int output = -1;
bool listen = true;
int nb = 0;
char **requests = NULL;

//  !!! Not properly working yet
int checkLockFile() {

    int fichierLock = 0;
    int locked = 0;
    int cpt = 0;

    while (locked) {

        if ((fichierLock = open(FICHIERLOCK, O_RDONLY)) == SUCCESS) {

            printf("lll");
            close(fichierLock);
            cpt++;

            if(cpt == MAX_TRY) {

                fatal("Lock is present for too long, must exit...\n");

            }
        } else {

            locked = -1;

        }
    }

    return SUCCESS;
}

int atis(char * atisMsg) {
    int fichierMeteo = 0;
    int atisSize = 0;
    char buf[MSG_SIZE];

    checkLockFile();
    fichierMeteo = open(FICHIERMETEO, O_RDONLY);

    if (fichierMeteo == FAIL) {

        fatal("Impossible d'ouvrir le fichier meteo");
    }
    /*
    //Si lock existe, on arrête
    if ((fichierLock = open(FICHIERLOCK, O_RDONLY)) == SUCCESS) {

    close(fichierLock);
    fatal("Lock is present, cannot reach meteo file !!\n");

    } else {
    //Sinon ouverture meteo
    fichierMeteo = open(FICHIERMETEO, O_RDONLY);

    //Si ouvrture fail, fail
    if (fichierMeteo == FAIL) {

    fatal("Impossible d'ouvrir le fichier meteo");

    }
    */
    //Ouverture meteo réussie, on  essaie de lire le message
    atisSize = (int)read(fichierMeteo, buf, sizeof(buf));

    if (atisSize == FAIL) {

        fatal("Impossible de lire le fichier meteo.txt");

    } else {

        strlcpy(atisMsg, buf, atisSize + 1);

    }

    return atisSize;
}

void createFifos(void) {
    if (mkfifo(FIFO_FILE, S_IRUSR | S_IWUSR) == FAIL) {

        fatal("Unable to create input fifo\n");

    }

    if (mkfifo(FIFO_FILE_OUT, S_IRUSR | S_IWUSR) == FAIL) {

        fatal("Unable to create output fifo\n");

    }
}

void openFifos(void) {
    if ((input = open(FIFO_FILE, O_RDWR)) == FAIL) {

        fatal("Unable to open the server's fifo\n");

    }

    if ((output = open(FIFO_FILE_OUT, O_RDWR)) == FAIL) {

        fatal("Unable to open server ouput fifo %s\n", FIFO_FILE_OUT);

    }
}

void operations(void) {
    // poll in the input server
    // for incomind data and for 
    // client disconnects.
    struct pollfd       fd[] = {
        { input, POLLIN | POLLHUP, 0 }
    };

    // request packet
    char                requestPacket[MSG_SIZE];
    requests=           NULL;

    // atis
    char                atisMsg[MSG_SIZE];

    printf("^C to exit\n- Listening...\n");

    while (listen) {

        // poll the input fifo every second
        int fifoActions = poll(fd, 1, 1000);

        if (fifoActions == FAIL) {

            if (listen) {
                printf("%s\n", "polling again...");
            }

        } else if (fifoActions == 0) {

            printf("- Listening...\n");

        } else {

            int packet = (int)read(input, requestPacket, MSG_SIZE);

            if (packet == FAIL) {

                if (fd[0].revents & POLLHUP) {

                    close(input);

                    if ((input = open(FIFO_FILE, O_RDWR)) == FAIL) {

                        fatal("Unable to open the input fifo.\n");

                    }
                }

            } else {

                // assign read string
                requests = xrealloc(requests, nb+1, MSG_SIZE);
                requests[nb] = xmalloc(MSG_SIZE);
                memcpy(requests[nb], requestPacket, packet);

                if (memcmp(requests[nb], PILOT_REQUEST, 
                            sizeof(PILOT_REQUEST)) == 0)  {

                    printf("< Got Request nr. %d\n", nb);

                    size_t tailleMsg = (size_t)atis(atisMsg);

                    printf("> Sending ATIS \"%s\" to %d\n", atisMsg, nb);

                    if (write(output, atisMsg, tailleMsg) == FAIL) {

                        fatal("Failed to send message...\n");

                    }

                    nb++;
                } else {

                    fatal("No valid messages intercepted\n");

                }
            }
        }
    }
}

int main(void) {
    // setup signal, so if programs gets 
    // interrupted, files can still be 
    // cleaned up and FIFOs removed.
    signal(SIGINT, &cleanup);

    createFifos();

    openFifos();

    operations();

    return EXIT_SUCCESS;
}
