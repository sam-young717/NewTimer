
#ifndef _timer_h_
#define _timer_h_

#include <stdint.h>
#include "platform_time.h"

/* timer structure */
struct timer_record
{
    platform_time_t starttime;
    platform_time_t endtime;
    unsigned int channel;
};

/* init/uninit routines for the timer */
void init_timer(void);
void uninit_timer(void);

/* adds a timer, queries user for info, returns ERROR_CODE on failure */
int32_t add_timer(void);

/* delete a timer */
void delete_timer_record(int32_t idx);

/* display list of all timers */
void list_timers(void);

#endif /* _timer_h_ */

