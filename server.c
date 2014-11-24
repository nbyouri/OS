#include "global.h"

int main(void) {
    // fifo file descriptor number
    int                 input = -1;

    // message to read
    fd_set              readset;
    struct timeval      tv;
    struct request *    req = NULL;
    unsigned int        req_n = 0;

    // setup signal, so if programs exits 
    // abruptly, the fifo still gets cleanup up
    sigset(SIGINT, &cleanup);

    // create the fifo
    if (mkfifo(FIFO_FILE, S_IRUSR | S_IWUSR) == FAIL) {

        fatal(input, "Was unable to create input fifo\n");
        
    }

    // open the fifo
    if ((input = open(FIFO_FILE, O_RDONLY)) == FAIL) {

        fatal(input, "Was unable to open the server's fifo\n");

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

                } else {
                    req = xrealloc(input, req, req_n+1, sizeof(struct request *));
                    req[req_n] = req_packet;

                    if (strnstr(req[req_n].msg, PILOT_REQUEST, req[req_n].siz) != NULL)  {
                        printf("req[%d] = %d -> %s :: %zu\n", req_n, req[req_n].pid, req[req_n].msg, req[req_n].siz);
                        req_n++;
                    }

                }
        }
    }

    clean_ptr(req);

    cleanup(input);
}



