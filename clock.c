
/*
 * Clock related functions
 */
#include <stdio.h>
#include <time.h>
#include "clock.h"
#include "consts.h"
#include "inout.h"

/*
 * Print the current time
 */
void display_time(void)
{
    char buf[BUF_SIZE];
    /* parasoft-suppress MISRAC2025-RULE_21_10-c "time_t at OS time API boundary" */
    time_t the_time;
    char *t;

    /* parasoft-suppress MISRAC2025-RULE_21_10-b "time() at OS time API boundary" */
    the_time = time(NULL);
    /* parasoft-suppress MISRAC2025-RULE_21_10-b "ctime() at OS time API boundary" */
    t = ctime(&the_time);
    (void)snprintf(buf, sizeof(buf), "\n\nCurrent Time and Date is %s\n\n", t);
    (void)print_string(buf);
}


/*
 * Dummy Function -- time always taken from system
 */
void set_time(platform_time_t new_time)
{
    (void)new_time;
}

