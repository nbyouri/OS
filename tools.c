/*
 *
 * Utility functions mostly taken from OpenBSD.
 * Author: Youri Mouton.
 *
 */
#include "global.h"

bool listen;
int input;
int output;
int nb;
char **requests;

/*
 * xmalloc is a wrapper around malloc that checks
 * if the size parameter is greater than zero, and 
 * if the returned pointer isn't null so the 
 * programmer doesn't have to do it in other files.
 *
 * Source: OpenSSH 6.6.1, adapted to use a modified 
 * fatal() function as we're not logging to syslog 
 * like OpenSSH does.
 *
 * Author: Tatu Ylonen
 */
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

/*
 * xrealloc is a wrapper around xmalloc that checks
 * if the number parameter is greater than zero and
 * if the total size is smaller than SIZE_T_MAX per
 * number.
 *
 * It also malloc's a new pointer if xreallocs pointer
 * parameter is null and then actually reallocs a 
 * pointer. It checks if the returning pointer is null
 * so the programmer doesn't have to do it in other
 * files.
 *
 * Source: OpenSSH 6.6.1, adapted to use a modified 
 * fatal() function as we're not logging to syslog 
 * like OpenSSH does.
 *
 * Author: Tatu Ylonen
 */
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

/*
 * cleaPtr frees a two dimensional array
 * of characters (an array of strings) by
 * looping through the char 2D array to 
 * the count parameter, free the array
 * content and set the content to NULL, 
 * to avoid dangling pointer bugs.
 *
 * If a dangling pointer (a pointer pointing
 * to nothing) is accessed after being freed, 
 * you may overwrite random memory, but if a 
 * NULL'ed pointer is accessed, a crash will 
 * occur, with an appropriate error message 
 * from the operating system.
 *
 * Author: Youri Mouton.
 */
void cleanPtr(int count, char ** array) {

    for (int i = 0; i < count; i++) {
        if (array[i] != NULL) {
            free(array[i]);
            array[i] = NULL;
        }
    }

    if (array != NULL) {
        free(array);
        array = NULL;
    }
}

/*
 * checkyesno asks for user input to decide 
 * whether to keep the program running or not 
 * and send a bool value accordingly. It will 
 * loop until the user presses either 'y' or 
 * 'n'. 'y' being yes, 'n' being no. The msg 
 * parameter will be the question shown to the user.
 *
 * Author: Youri Mouton
 */
bool checkyesno(const char *msg) {
    char        inp[BUFSIZ];
    char        res;

    do {

        printf("\n%s%s", msg, " ? (["RED"y"NOR"]es/["RED"n"NOR"]o) : ");
        fgets(inp, sizeof(inp), stdin);
        sscanf(inp, "%c", &res);

    } while (res != 'y' && res != 'n');

    if (res == 'y') {
        listen = false;
    }

    return listen;
}

/*
 * delete is a procedure that will try to 
 * remove file name after the pathname 
 * parameter and throw an appropriate error
 * message if the file doesn't exist or if 
 * we can't delete it.
 *
 * Author: Youri Mouton
 *         Samuel Monroe
 */
void delete(const char * pathname) {
    struct stat         info;

    if (stat(pathname, &info) == FAIL) {
        fatal("File %s doesn't exist!\n", pathname);
    } else {
        if (unlink(pathname) == -1) {
            fatal("Failed to delete %s !\n", pathname);
        }
    }
}

/*
 * cleanup is a procedure that makes sure to 
 * clean the file descriptors and to delete 
 * the named pipes and the pointers used. 
 *
 * cleanup will run checkyesno(msg) if the 
 * program returns normally or if the user 
 * interrups the program (ctrl+c, for example).
 * If the programs terminates abnormally, 
 * cleanup will be run directly.
 *
 * cleanup has a state parameter that can be 
 * set to decide whether we're calling cleanup
 * when the program is failing to make an 
 * operation or if we're exiting normally.
 *
 * cleanup is called from the server's main
 * on SIGINT, so when the user interrupts 
 * the process, it is fine to call it from 
 * signal() or sigset() from other places, 
 * but be careful to not cleanup files needed
 * by the other processes.
 *
 * Author: Youri Mouton
 */
void cleanup(int state) {

    if (state == EXIT_SUCCESS || state == SIGINT) {
        listen = checkyesno("Really quit");
    } else if (state == EXIT_FAILURE) {
        listen = false;
    }

    if (!listen) {

        printf("\ncleaning up... \n");

        if (input != -1 || output != -1) {

            if (close(input) == FAIL) {
                printf("Couldn't close input file descriptor %d\n", input);
            }

            if (close(output) == FAIL) {
                printf("Couldn't close output file descriptor %d\n", output);
            }
        }

        delete(FIFO_FILE);

        delete(FIFO_FILE_OUT);

        cleanPtr(nb, requests);
    }

}

/*
 * fatal is an error handling function,
 * it wraps a printf function that will  
 * display the error message set by the 
 * programmer as parameter, prepended
 * by a 'FATAL:' message printed in red.
 *
 * The error message set when calling
 * fatal is variadic, it can be used 
 * like any printf.
 *
 * fatal then cleanup and exits with a
 * failure state, so fatal should only
 * be used for error handling.
 * 
 * Author: Youri Mouton
 */
int fatal(const char * format, ...) {    

    va_list     args;

    va_start(args, format);

    fprintf(stderr, RED"FATAL: "NOR);
    vfprintf(stderr, format, args);

    va_end(args);

    cleanup(EXIT_FAILURE);

    exit(EXIT_FAILURE);
}
