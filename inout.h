
#ifndef _input_h_
#define _input_h_

#include <stdint.h>

/* gets a digit from the input device */
int32_t get_input_digit(void);

/* prints a string to the output device */
int32_t print_string(const char* str);

/* prints a string array to the output device */
int32_t print_string_array(void);

#endif /* _input_h_ */

