#include "global.h"

char * atis(struct request rq) {
    char atis_msg[MSG_SIZE];

    snprintf(atis_msg, MSG_SIZE, "%d :> (ATIS_DATA)", rq.pid);

    return atis_msg;
}

int main(void) {
    // fifo file descriptor number
    int                 input = -1;
    int                 output = -1;

    // message to read
    fd_set              readset;
    struct timeval      tv;

    // requests
    struct request *    req = NULL;
    unsigned int        req_n = 0;

    // atis 

    // setup signal, so if programs exits 
    // abruptly, the fifo still gets cleanup up
    sigset(SIGINT, &cleanup);

    // create the fifo
    if (mkfifo(FIFO_FILE, S_IRUSR | S_IWUSR) == FAIL) {

        fatal(input, "Was unable to create input fifo\n");
        
    }

    if (mkfifo(FIFO_FILE_OUT, S_IRUSR | S_IWUSR) == FAIL) {
        fatal(output, "Unable to create output fifo\n");
    }

    // open the fifo in read/write mode
    if ((input = open(FIFO_FILE, O_RDONLY)) == FAIL) {

        fatal(input, "Was unable to open the server's fifo\n");

    }

    if ((output = open(FIFO_FILE_OUT, O_WRONLY)) == FAIL) {
        fatal(output, "Unable to open server ouput fifo %s\n", FIFO_FILE_OUT);
    }

    // refresh the loop every second
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    while (listen) {
        // zero readset and set it to input
        FD_ZERO(&readset);
        FD_SET(input, &readset);

        if (select (input+1, &readset, NULL, NULL, &tv) > 0) {

                struct request req_packet;
                int packet = (int)read(input, &req_packet, sizeof(req_packet));

                if (packet == FAIL) {

                    printf("couldn't read message\n");

                } else if (packet == FIFO_EOF) {

                    //listen = checkyesno("Keep listening");
                    sleep(1);

                } else {
                    req = xrealloc(input, req, req_n+1, sizeof(struct request *));
                    req[req_n] = req_packet;

                    if (strnstr(req[req_n].msg, PILOT_REQUEST, req[req_n].siz) != NULL)  {

                        char request_string[MSG_SIZE];
                        snprintf(request_string, MSG_SIZE, 
                                "req[%02d] = %d -> %s :: %zu\n", 
                                req_n, req[req_n].pid, req[req_n].msg, req[req_n].siz);

                        if (write(STDOUT_FILENO, request_string, strnlen(request_string, MSG_SIZE)) == FAIL) {
                            fatal(input, "Failed to write request string...\n");
                        }

                        char m[MSG_SIZE]; 
                        memcpy(m, atis(req[req_n]), MSG_SIZE);
                        printf("Sending Packet \"%s\"...\n", m);
                        if (write(output, m, strnlen(m, MSG_SIZE)) == FAIL) {
                            fatal(output, "Failed to send message...\n");
                        }

                        req_n++;
                    } else {
                        fatal(input, "No valid messages intercepted\n");
                    }
                }
        }
    }

    clean_ptr(req);

    cleanup(input);
    cleanup(output);
}
