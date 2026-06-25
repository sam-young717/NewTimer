
/*
 * Clock related functions
 */
#include <stdio.h>
#include <stdint.h>
#include "clock.h"
#include "consts.h"
#include "inout.h"
#include "platform_time.h"

/*
 * Print the current time
 */
void display_time(void)
{
    char buf[BUF_SIZE];
    platform_time_t the_time = platform_get_current_time();
    char* t = platform_time_to_string(the_time);

    (void)snprintf(buf, (size_t)BUF_SIZE, "\n\nCurrent Time and Date is %s\n\n", t);
    (void)print_string(buf);
}


/* 
 * Dummy Function -- time always taken from system
 */
void set_time(platform_time_t new_time)
{
    (void)new_time;
}

