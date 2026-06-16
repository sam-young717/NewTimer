
#ifndef PLATFORM_TIME_H
#define PLATFORM_TIME_H

#include <stdint.h>

typedef int64_t platform_time_t;

platform_time_t platform_get_current_time(void);
char* platform_time_to_string(platform_time_t t);
platform_time_t platform_make_time(int32_t hour, int32_t min);
void platform_format_time(platform_time_t t, char* buf, int32_t buf_size, const char* fmt);

#endif /* PLATFORM_TIME_H */
