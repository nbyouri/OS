
#define FIFO_FILE "watchtower.fifo"

struct ATIS {
    size_t length;
    char msg[BUFSIZ];
};
