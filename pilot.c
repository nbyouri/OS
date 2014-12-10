#include "global.h"

#define VALID_ATIS "ATIS"
#define VALID_LGT 4
#define MAX_TRY 10
#define WAIT_RETRY 1
void pilot_cleanup(int, int, int);

int main(void) {
    int server = -1;
    int out_server = -1;
    int not_received = -1;
    int total_nak = 0;

    char request[MSG_SIZE];
    char confirm[MSG_SIZE];
    char buf[MSG_SIZE];
    char response[MSG_SIZE];
    size_t responseSize = 0;

    memcpy(request, PILOT_REQUEST, MSG_SIZE);

    if((server = open(FIFO_FILE, O_WRONLY)) == FAIL) {
        printf(RED"Server seems to be down...\n"NOR);
    } else {
        printf("Sending REQUEST...\n");

        if (write(server, request, sizeof(request)) == FAIL) {
            printf("Failed to write message\n");
            pilot_cleanup(server, out_server, FAIL);
        } else {

            if ((out_server = open(FIFO_FILE_OUT, O_RDONLY)) == FAIL) {
                printf("Couldn't open output file...\n");
                pilot_cleanup(server, out_server, FAIL);

            } else {
                
                while (not_received) {
                    responseSize = read(out_server, buf, sizeof(buf));
                    if (responseSize == FAIL) {
                        printf("Failed to read response from output fifo\n");
                        pilot_cleanup(server, out_server, FAIL);

                    } else {
                        //Response received from the server
                        memcpy(response, buf, responseSize);
                    
                        //Is the response valid?
                        if (memcmp(response, VALID_ATIS, VALID_LGT) == 0) {
                            printf("Got response ! => %s\n", response);
                        
                            if (write(server, ACK, sizeof(ACK)) == FAIL) {
                                printf("Failed to send ACK");
                            }
                            not_received = 0;
                        
                        } else {
                            
                            if (total_nak == MAX_TRY) {
                                printf(RED"ERROR : "NOR);
                                printf("Server seems unable to treat requests anymore, quitting... \n");
                                pilot_cleanup(server, out_server, EXIT_FAILURE);
                                
                            } else {
                                printf(RED"ERROR : %s\n"NOR, response);
                                printf("Sending NAK to the serveur, asking for ATIS again... \n");
                            
                                if (write(server, NAK, sizeof(NAK)) == FAIL) {
                                    printf("FAILED to send NAK");
                                }
                                total_nak++;
                                sleep(WAIT_RETRY);
                            }
                        }
                    }
                }
            }
        }
        pilot_cleanup(server, out_server, EXIT_SUCCESS);
    }
}


void pilot_cleanup(int in_serv, int out_serv, int status) {
    if (in_serv != FAIL) {
        if (close(in_serv) == FAIL) {
            printf("Couldn't close input file descriptor %d\n", in_serv);
            exit(status);
        }
    }
    if (out_serv != FAIL) {
        if (close(out_serv) == FAIL) {
            printf("Couldn't close output file descriptor %d\n", out_serv);
            exit(status);
        }
    }
    exit(EXIT_SUCCESS);
}

