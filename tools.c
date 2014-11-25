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

void * xrealloc(void *ptr, size_t nmemb, size_t size)
{
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

void * clean_ptr(void * pointer) {
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

void cleanup(int state) {
    struct stat         info;

    printf("\ncleaning up... \n");
    printf("Files to be closed : %d, %d\n", input, output);
    printf("Files to be deleted : %s, %s\n", FIFO_FILE, FIFO_FILE_OUT);

    // close the file descriptors
    //if (input != FAIL && output != FAIL) {
        if (close(input) == FAIL) {
            printf("Couldn't close input file descriptor %d\n", input);
        } else {
            printf("Successfully closed %d\n", input);
        }

        if (close(output) == FAIL) {
            printf("Couldn't close output file descriptor %d\n", output);
        } else {
            printf("Successfully cloded %d\n", output);
        }

        // delete the fifo input file if it exists
        if (unlink(FIFO_FILE) == FAIL) {
            if (stat(FIFO_FILE, &info) == FAIL) {
                printf("The fifo file doesn't exist\n");
            } else {
                printf("Couldn't delete fifo %s\n", FIFO_FILE);
            }
            exit(EXIT_FAILURE);
        } else {
            printf("Successfully removed %s\n", FIFO_FILE);
        }

        // delete the fifo output file if it exists
        if (unlink(FIFO_FILE_OUT) == FAIL) {
            if (stat(FIFO_FILE_OUT, &info) == FAIL) {
                printf("The output fifo file doesn't exist\n");
            } else {
                printf("Couldn't delete fifo %s\n", FIFO_FILE_OUT);
            }
            exit(EXIT_FAILURE);
        } else {
            printf("Successfully removed %s\n", FIFO_FILE_OUT);
        }
   // }

    // actually exit
    exit(state);

}

void fatal(const char * restrict format, ...) {    
    va_list     args;

    va_start(args, format);

    fprintf(stderr, RED"FATAL: "NOR);
    vfprintf(stderr, format, args);
    
    va_end(args);

    // delete the fifo files
    cleanup(EXIT_FAILURE);
}
