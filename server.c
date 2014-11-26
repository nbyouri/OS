#include "global.h"

int input = -1;
int output = -1;
bool listen = true;

void atis(char * atis_msg, struct request rq) {

    snprintf(atis_msg, MSG_SIZE, "%d :> (ATIS_DATA)", rq.pid);

}

int main(void) {
    // message to read
    fd_set              readset;
    fd_set              readaction;
    struct timeval      tv;
    struct timeval      tv_read;

    // requests
    struct request *    req = NULL;
    unsigned int        req_n = 0;
    char                request_string[MSG_SIZE];

    // atis
    char                atis_msg[MSG_SIZE];

    // setup signal, so if programs exits 
    // abruptly, the fifo still gets cleanup up
    sigset(SIGINT, &cleanup);

    // create the fifos
    if (mkfifo(FIFO_FILE, S_IRUSR | S_IWUSR) == FAIL) {

        fatal("Unable to create input fifo\n");

    }

    if (mkfifo(FIFO_FILE_OUT, S_IRUSR | S_IWUSR) == FAIL) {

        fatal("Unable to create output fifo\n");

    }

    // open the fifos in read/write mode
    if ((input = open(FIFO_FILE, O_RDONLY)) == FAIL) {

        fatal("Unable to open the server's fifo\n");

    } else {

        printf("FD input = %d\n", input);
    }

    if ((output = open(FIFO_FILE_OUT, O_WRONLY)) == FAIL) {

        fatal("Unable to open server ouput fifo %s\n", FIFO_FILE_OUT);

    } else {

        printf("FD output = %d\n", output);
    }

    // refresh the loop every second
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    while (listen) {
        // zero readset and set it to input
        FD_ZERO(&readset);
        FD_SET(input, &readset);

        int fifo_actions = select(input+1, &readset, NULL, NULL, &tv);

        if (fifo_actions == FAIL) {
            fatal("main stream select failed\n");
        } else if (fifo_actions == 0) {
            printf("En écoute...\n");
        } else {
            // read every microsecond
            tv_read.tv_sec = 0;
            tv_read.tv_usec = 1;
            FD_ZERO(&readaction);
            FD_SET(fifo_actions, &readaction);

            select(fifo_actions+1, &readset, NULL, NULL, &tv_read);

            struct request req_packet;
            int packet = (int)read(input, &req_packet, sizeof(req_packet));

            if (packet == FAIL) {

                printf("couldn't read message\n");

            } else if (packet == FIFO_EOF) {

                //listen = checkyesno("Keep listening");

            } else {

                // assign read structure
                req = xrealloc(req, req_n+1, sizeof(struct request));
                req[req_n] = req_packet;

                if (strnstr(req[req_n].msg, PILOT_REQUEST, req[req_n].siz) != NULL)  {

                    snprintf(request_string, MSG_SIZE, 
                            "Got Request ! : req[%02d] = %d -> %s :: %zu\n", 
                            req_n, req[req_n].pid, req[req_n].msg, req[req_n].siz);

                    if (write(STDOUT_FILENO, request_string, strnlen(request_string, MSG_SIZE)) == FAIL) {
                        fatal("Failed to write request string...\n");
                    }

                    atis(atis_msg, req[req_n]);
                    printf("Sending Packet \"%s\"...\n", atis_msg);
                    if (write(output, atis_msg, strnlen(atis_msg, MSG_SIZE)) == FAIL) {
                        fatal("Failed to send message...\n");
                    }

                    req_n++;
                } else {
                    fatal("No valid messages intercepted\n");
                }
            }
        }
    }

    clean_ptr(req);

    cleanup(EXIT_SUCCESS);
}
