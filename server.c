#include "global.h"

int input = -1;
int output = -1;
bool listen = true;

int atis(char * atisMsg) {
    int fichierMeteo = 0;
    int fichierLock = 0;
    int tailleMessage = 0;

    char dataAtis [MSG_SIZE];

    if ((fichierLock = open(FICHIERLOCK, O_RDONLY)) == FAIL) {

        fatal("Failed to open fichier lock\n");

    } else {

        fichierMeteo = open(FICHIERMETEO, O_RDONLY);

        if (fichierMeteo == FAIL) {

            fatal("Impossible d'ouvrir le fichier meteo");

        }
    }

    tailleMessage = (int)read(fichierMeteo, dataAtis, MSG_SIZE);

    if (tailleMessage == FAIL) {

        fatal("Impossible de lire le fichier meteo.txt");

    }

    memcpy(atisMsg, dataAtis, MSG_SIZE);

    return tailleMessage;
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
    struct pollfd       fd[1] = {
        { input, POLLIN, 0 }
    };

    // requests
    struct request *    req = NULL;
    unsigned int        reqNum = 0;

    // atis
    char                atisMsg[MSG_SIZE];

    while (listen) {

        // poll the input fifo every second
        int fifoActions = poll(fd, 1, 1000);

        if (fifoActions == FAIL) {

            fatal("poll failed\n");

        } else if (fifoActions == 0) {

            printf("- Listening...\n");

        } else {

            struct request requestPacket;
            int packet = (int)read(input, &requestPacket, sizeof(requestPacket));

            if (packet == FAIL) {

                if (fd[0].revents & POLLHUP) {

                    printf("- Client disconnected\n");
                    close(input);

                    if ((input = open(FIFO_FILE, O_RDONLY|O_NONBLOCK)) == FAIL) {

                        fatal("Unable to open the input fifo.\n");
                    }
                }

            } else {

                printf("- Starting transmission...\n");

                // assign read structure
                req = xrealloc(req, reqNum+1, sizeof(struct request));
                req[reqNum] = requestPacket;

                if (strstr(req[reqNum].msg, PILOT_REQUEST) != NULL)  {

                    printf("< Got Request nr. %d\n", reqNum);

                    size_t tailleMsg = (size_t)atis(atisMsg);
                    printf("> Sending ATIS \"%s\"...\n", atisMsg);

                    if (write(output, atisMsg, tailleMsg) == FAIL) {

                        fatal("Failed to send message...\n");

                    }

                    printf("- Finishing transmission...\n");
                    reqNum++;
                } else {

                    fatal("No valid messages intercepted\n");

                }
            }
        }
    }
    cleanPtr(req);
}

int main(void) {
    // setup signal, so if programs gets 
    // interrupted, files can still be 
    // cleaned up and FIFOs removed.
    signal(SIGINT, &cleanup);

    genAtis();

    createFifos();

    openFifos();

    operations();

    cleanup(EXIT_SUCCESS);
}
