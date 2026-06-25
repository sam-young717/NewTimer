
/*
 * Timer class, stores timer records
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "clock.h"
#include "consts.h"
#include "inout.h"
#include "timer.h"
#include "platform_time.h"

/* Forward declarations for file-scope functions */
static struct timer_record query_user(void);
static void add_timer_record(struct timer_record tr);
static void format_timer_record(int32_t idx, char* buf);

static struct timer_record timer_records[BUF_SIZE];
static int32_t curr_index = 0;

void init_timer(void)
{
    (void)memset(timer_records, 0, sizeof(timer_records));
}

void uninit_timer(void)
{
    curr_index = 0;
}

/*
 * Queries user for timer information
 */
static struct timer_record query_user(void)
{
    struct timer_record the_record;
    int32_t hour;
    int32_t min;

    (void)memset(&the_record, 0, sizeof(the_record));

    /* starttime */
    (void)print_string("Please enter the start hour [0-23] > ");
    hour = get_input_digit();
    (void)print_string("Please enter the start minute [0-59] > ");
    min = get_input_digit();
    the_record.starttime = platform_make_time(hour, min);

    /* endtime */
    (void)print_string("\nPlease enter the end hour [0-23] > ");
    hour = get_input_digit();
    (void)print_string("\nPlease enter the end minute [0-59] > ");
    min = get_input_digit();
    the_record.endtime = platform_make_time(hour, min);

    /* channel */
    (void)print_string("\nPlease enter the channel to record > ");
    the_record.channel = (unsigned int)get_input_digit();

    return the_record;
}

int32_t add_timer(void)
{
    struct timer_record record = query_user();
    add_timer_record(record);
    return 0;
}

static void add_timer_record(struct timer_record tr)
{
    if (curr_index < BUF_SIZE) {
        timer_records[curr_index] = tr;
        curr_index++;
    } else {
        (void)print_string("\nAll timers used ... timer not added\n");
    }
}

/*
 * Removes record at idx
 * Moves all records past idx up one slot
 */
void delete_timer_record(int32_t idx)
{
    int32_t i;

    if ((idx >= 0) && (idx < curr_index)) {
        for (i = idx; i < (curr_index - 1); i++) {
            timer_records[i] = timer_records[i + 1];
        }
        curr_index--;
    }
}

static void format_timer_record(int32_t idx, char* buf)
{
    char start[BUF_SIZE];
    char end_time[BUF_SIZE];

    platform_format_time(timer_records[idx].starttime, start, BUF_SIZE, "%I:%M %p");
    platform_format_time(timer_records[idx].endtime, end_time, BUF_SIZE, "%I:%M %p");
    (void)snprintf(buf, (size_t)BUF_SIZE, "%d\t%s\t%s\t%u\n",
                   (int32_t)(idx + 1), start, end_time, timer_records[idx].channel);
}

void list_timers(void)
{
    char buf[BUF_SIZE];
    int32_t i;

    (void)print_string("\n\nCurrent Set Timers");
    (void)print_string("\nRecord#\tStart Time\tEnd Time\tChannel\n");
    for (i = 0; i < curr_index; i++) {
        format_timer_record(i, buf);
        (void)print_string(buf);
    }
    (void)print_string("\n\n");
}

