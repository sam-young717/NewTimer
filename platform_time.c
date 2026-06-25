
/*
 * Platform abstraction layer for time functions.
 * Isolates time.h usage from MISRA-checked application code.
 */
#include <time.h>
#include <stdint.h>
#include "platform_time.h"

platform_time_t platform_get_current_time(void)
{
    return (platform_time_t)time(NULL);
}

char* platform_time_to_string(platform_time_t t)
{
    time_t raw = (time_t)t;
    return ctime(&raw);
}

platform_time_t platform_make_time(int32_t hour, int32_t min)
{
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    t->tm_hour = (int)hour;
    t->tm_min  = (int)min;
    t->tm_sec  = 0;
    return (platform_time_t)mktime(t);
}

void platform_format_time(platform_time_t t, char* buf, int32_t buf_size, const char* fmt)
{
    time_t raw = (time_t)t;
    struct tm* tm_ptr = localtime(&raw);
    (void)strftime(buf, (size_t)buf_size, fmt, tm_ptr);
}
