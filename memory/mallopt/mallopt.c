// From man 3 mallopt

/*
The example below demonstrates the use of M_CHECH_ACTION. If the
program is supplied with an interger command line argument, then
that argument is used to set the M_CHECK_ACTION parameter. The
program then allocates a block of memory and frees it twice (an
error)
*/

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    char *p;

    if (argc > 1) {
        if (mallopt(M_CHECK_ACTION, atoi(argv[1])) != 1) {
            fprintf(stderr, "mallopt() failed");
            exit(EXIT_FAILURE);
        }
    }

    p = malloc(1000);
    if (p == NULL) {
        fprintf(stderr, "malloc() failed");
        exit(EXIT_FAILURE);
    }

    free(p);
    printf("main(): returned from first free() call\n");
    free(p);
    printf("main(): returned from second free() call\n");

    exit(EXIT_SUCCESS);
}