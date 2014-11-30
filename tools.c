/*
 *
 * Utility functions mostly taken from OpenBSD.
 * Author: Youri Mouton.
 *
 */
#include "global.h"

int input;
int output;

void * xmalloc(size_t size) {
    void *ptr;

    if (size == 0) {
        fatal("xmalloc: zero size\n");
    }

    ptr = malloc(size);

    if (ptr == NULL) {
        fatal("xmalloc: out of memory (allocating %zu bytes)", size);
    }

    return ptr;
}

void * xrealloc(void *ptr, size_t nmemb, size_t size) {
    void *new_ptr;
    size_t new_size = nmemb * size;

    if (new_size == 0)
        fatal("xrealloc: zero size");
    if (SIZE_T_MAX / nmemb < size)
        fatal("xrealloc: nmemb * size > SIZE_T_MAX");
    if (ptr == NULL)
        new_ptr = malloc(new_size);
    else
        new_ptr = realloc(ptr, new_size);
    if (new_ptr == NULL)
        fatal("xrealloc: out of memory (new_size %zu bytes)",
                new_size);
    return new_ptr;
}

void * cleanPtr(void * pointer) {
    if (pointer != NULL) {
        free(pointer);
    }

    return pointer = NULL;
}

bool checkyesno(const char *msg) {
    char        inp[BUFSIZ];
    char        res;

    do {

        printf("%s %s", msg, " ? ([o]ui/[n]on) : ");
        fgets(inp, sizeof(inp), stdin);
        sscanf(inp, "%c", &res);

    } while (res != 'n' && res != 'o');

    if (res == 'n') {
        listen = false;
    }

    return listen;
}

int delete(const char * pathname) {
    struct stat         info;

    if (stat(pathname, &info) == FAIL) {
        return fatal("File %s doesn't exist!\n", pathname);
    } else {
        return unlink(pathname);
    }
}

void cleanup(int state) {

    printf("\ncleaning up... \n");

    if (input != -1 || output != -1) {
        // close the file descriptors
        if (close(input) == FAIL) {
            printf("Couldn't close input file descriptor %d\n", input);
        }

        if (close(output) == FAIL) {
            printf("Couldn't close output file descriptor %d\n", output);
        }
    }

    // delete the fifo input file if it exists
    delete(FIFO_FILE);

    delete(FIFO_FILE_OUT);

    delete(FICHIERMETEO);

    delete(FICHIERLOCK);

    // actually exit
    exit(state);

}

int fatal(const char * restrict format, ...) {    
    va_list     args;

    va_start(args, format);

    fprintf(stderr, RED"FATAL: "NOR);
    vfprintf(stderr, format, args);

    va_end(args);

    // delete the fifo files
    cleanup(EXIT_FAILURE);

    // shouldn't be reached
    return EXIT_SUCCESS;
}
