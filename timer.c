
/*
 * Timer class, stores timer records
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "clock.h"
#include "consts.h"
#include "inout.h"
#include "timer.h"


static struct timer_record  record_pool[BUF_SIZE];
static uint8_t              pool_used[BUF_SIZE];
static struct timer_record* timer_records[BUF_SIZE];
static int32_t              curr_index = 0;

/* forward declarations for internal functions */
static struct timer_record* alloc_record(void);
static void                 free_record(struct timer_record* tr);
static struct timer_record* query_user(void);
static void                 add_timer_record(struct timer_record* tr);
static void                 format_timer_record(int32_t idx, char* buf);

static struct timer_record* alloc_record(void)
{
    int32_t i;
    struct timer_record* result = NULL;
    for (i = 0; i < BUF_SIZE; i++) {
        if (pool_used[i] == 0U) {
            pool_used[i] = 1U;
            result = &record_pool[i];
            break;
        }
    }
    return result;
}

static void free_record(struct timer_record* tr)
{
    int32_t i;
    for (i = 0; i < BUF_SIZE; i++) {
        if (&record_pool[i] == tr) {
            pool_used[i] = 0U;
            break;
        }
    }
}

void init_timer(void)
{
    (void)memset(timer_records, 0, sizeof(timer_records));
    (void)memset(record_pool,   0, sizeof(record_pool));
    (void)memset(pool_used,     0, sizeof(pool_used));
}

void uninit_timer(void)
{
    int32_t i;
    for (i = 0; i < curr_index; i++) {
        free_record(timer_records[i]);
        timer_records[i] = NULL;
    }
    curr_index = 0;
}

/*
 * Queries user for timer information
 */
static struct timer_record* query_user(void)
{
    struct timer_record* the_record = alloc_record();

    if (the_record != NULL) {
        (void)memset(the_record, 0, sizeof(struct timer_record));

        {
            /* parasoft-suppress MISRAC2025-RULE_21_10-b MISRAC2025-RULE_21_10-c "time API boundary" */
            time_t sys_time = (time_t)time(NULL);
            /* parasoft-suppress MISRAC2025-RULE_21_10-b MISRAC2025-RULE_21_10-c "localtime at time API boundary" */
            struct tm* tm_tmp = localtime(&sys_time);

            /* starttime */
            (void)print_string("Please enter the start hour [0-23] > ");
            tm_tmp->tm_hour = (int)get_input_digit();
            (void)print_string("Please enter the start minute [0-59] > ");
            tm_tmp->tm_min = (int)get_input_digit();
            /* parasoft-suppress MISRAC2025-RULE_21_10-b "mktime at time API boundary" */
            the_record->starttime = (platform_time_t)mktime(tm_tmp);

            /* endtime */
            (void)print_string("\nPlease enter the end hour [0-23] > ");
            tm_tmp->tm_hour = (int)get_input_digit();
            (void)print_string("\nPlease enter the end minute [0-59] > ");
            tm_tmp->tm_min = (int)get_input_digit();
            /* parasoft-suppress MISRAC2025-RULE_21_10-b "mktime at time API boundary" */
            the_record->endtime = (platform_time_t)mktime(tm_tmp);
        }

        /* channel */
        (void)print_string("\nPlease enter the channel to record > ");
        the_record->channel = (uint32_t)get_input_digit();
    }

    return the_record;
}

int32_t add_timer(void)
{
    struct timer_record* record;
    int32_t result = 0;

    record = query_user();
    if (record != NULL) {
        add_timer_record(record);
    } else {
        result = ERROR_CODE;
    }
    return result;
}

static void add_timer_record(struct timer_record* tr)
{
#ifdef OUTPUT
    {
        char buf[50];
        (void)snprintf(buf, sizeof(buf), "Curr Index = %d\n", curr_index);
        _EB_SEND(buf)
    }
#endif
    if (curr_index < BUF_SIZE) {
        timer_records[curr_index] = tr;
        curr_index++;
    } else {
        (void)print_string("\nAll timers used ... timer not added\n");
    }
}

/*
 * Removes record at idx.
 * Shifts all records past idx up one slot.
 */
void delete_timer_record(int32_t idx)
{
    struct timer_record* tr = timer_records[idx];
    int32_t i;

    for (i = idx; i < (curr_index - 1); i++) {
        timer_records[i] = timer_records[i + 1];
    }
    timer_records[curr_index - 1] = NULL;
    curr_index--;
    free_record(tr);
}

static void format_timer_record(int32_t idx, char* buf)
{
    struct timer_record* tr = timer_records[idx];

    if (tr != NULL) {
        char start[BUF_SIZE];
        char end_s[BUF_SIZE];
        /* parasoft-suppress MISRAC2025-RULE_21_10-b MISRAC2025-RULE_21_10-c "strftime/localtime at time API boundary" */
        time_t start_t = (time_t)tr->starttime;
        /* parasoft-suppress MISRAC2025-RULE_21_10-b "strftime/localtime at time API boundary" */
        (void)strftime(start, sizeof(start), "%I:%M %p", localtime(&start_t));
        /* parasoft-suppress MISRAC2025-RULE_21_10-b MISRAC2025-RULE_21_10-c "strftime/localtime at time API boundary" */
        time_t end_t = (time_t)tr->endtime;
        /* parasoft-suppress MISRAC2025-RULE_21_10-b "strftime/localtime at time API boundary" */
        (void)strftime(end_s, sizeof(end_s), "%I:%M %p", localtime(&end_t));
        (void)snprintf(buf, (size_t)BUF_SIZE, "%d\t%s\t%s\t%u\n",
                       (int)(idx + 1), start, end_s, tr->channel);
    }
}

void list_timers(void)
{
    char buf[BUF_SIZE];
    int32_t i;
    (void)print_string("\n\nCurrent Set Timers");
    (void)print_string("\nRecord#\tStart Time\tEnd Time\tChannel\n");
    for (i = 0; i < curr_index; i++)
    {
        format_timer_record(i, buf);
        (void)print_string(buf);
    }
    (void)print_string("\n\n");
}

