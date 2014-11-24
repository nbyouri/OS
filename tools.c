/*
 *
 * Utility functions mostly taken from OpenBSD.
 * Author: Youri Mouton.
 *
 */
#include "global.h"

void * xmalloc(int fd, size_t size) {
    void *ptr;

    if (size == 0) {
         fatal(fd, "xmalloc: zero size\n");
    }

    ptr = malloc(size);

    if (ptr == NULL) {
        fatal(fd, "xmalloc: out of memory (allocating %zu bytes)", size);
    }

    return ptr;
}

void * xrealloc(int fd, void *ptr, size_t nmemb, size_t size)
{
    void *new_ptr;
    size_t new_size = nmemb * size;

    if (new_size == 0)
        fatal(fd, "xrealloc: zero size");
    if (SIZE_T_MAX / nmemb < size)
        fatal(fd, "xrealloc: nmemb * size > SIZE_T_MAX");
    if (ptr == NULL)
        new_ptr = malloc(new_size);
    else
        new_ptr = realloc(ptr, new_size);
    if (new_ptr == NULL)
        fatal(fd, "xrealloc: out of memory (new_size %zu bytes)",
            new_size);
    return new_ptr;
}

void * clean_ptr(void * pointer) {
    if (pointer != NULL) {
        free(pointer);
    }

    return pointer = NULL;
}

bool checkyesno(const char *msg) {
    char        input[BUFSIZ];
    char        res;

    do {

        printf("%s %s", msg, " ? ([o]ui/[n]on) : ");
        fgets(input, sizeof(input), stdin);
        sscanf(input, "%c", &res);

    } while (res != 'n' && res != 'o');

    if (res == 'n') {
        listen = false;
    }
    
    return listen;
}

void cleanup(int fd) {
    struct stat         info;

    printf("\ncleaning up...\n");

    // close the file descriptor
    if (fd != FAIL) {
        if (close(fd) == FAIL) {
            printf("Couldn't close file descriptor %d\n", fd);
        }


        // delete the fifo file if it exists
        if (unlink(FIFO_FILE) == FAIL) {
            if (stat(FIFO_FILE, &info) == FAIL) {
                printf("The fifo file doesn't exist\n");
            } else {
                printf("Couldn't delete fifo\n");
            }
            exit(EXIT_FAILURE);
        }
    }

    // actually exit
    exit(EXIT_SUCCESS);

}

void fatal(int fd, const char * restrict format, ...) {    
    va_list     args;

    va_start(args, format);

    fprintf(stderr, RED"FATAL: "NOR);
    vfprintf(stderr, format, args);
    
    va_end(args);

    cleanup(fd);
}
