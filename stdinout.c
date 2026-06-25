
/* 
 * Implements routines from inout.h for stdin/stdout
 */

#include "inout.h"
#include "consts.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
 * Grabs input from stdin
 */
int32_t get_input_digit(void)
{
    int32_t i;
    char buf[BUF_SIZE];

#ifdef STDINPUT
    /* define STDINPUT to be able to accept input from a console */
    char c;
    for (i = 0; i < BUF_SIZE; i++) {
        c = (char)getchar();
        if (c == '\n' || c == (char)EOF) {
            break;
        } else if (isdigit((int)c) != 0) {
            buf[i] = c;
        }
    }
#else /* STDINPUT */

    /* to allow automated unit testing */
    static int32_t index = 0;

    /* simulate user input */
    ++ index;
    index %= 10;
    buf[0] = (char)('0' + index);
    i = 1;

#endif /* STDINPUT */
    
    buf[i] = '\0';
    return (int32_t)strtol(buf, NULL, 10);
}

/*
 * Print string to stdout
 */
int32_t print_string(const char* str)
{
    return (int32_t)fputs(str, stdout);
}
