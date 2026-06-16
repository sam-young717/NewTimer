
/*
 * driver class to test the timer.
 */

#include <stdio.h>
#include <stdint.h>
#include "clock.h"
#include "consts.h"
#include "inout.h"
#include "timer.h"

/* Forward declarations for file-scope functions */
static int32_t print_menu_get_action(void);
static void main_loop(void);

static int32_t print_menu_get_action(void)
{
    (void)print_string("**************************************************\n");
    (void)print_string("*                  Timer Menu                    *\n");
    (void)print_string("* -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *\n");
    (void)print_string("*                                                *\n");
    (void)print_string("* 1) Add a timer                                 *\n");
    (void)print_string("* 2) Remove a timer                              *\n");
    (void)print_string("* 3) List all timers                             *\n");
    (void)print_string("* 4) Show time                                   *\n");
    (void)print_string("*                                                *\n");
    (void)print_string("* 9) Exit                                        *\n");
    (void)print_string("*                                                *\n");
    (void)print_string("**************************************************\n\n");

    (void)print_string("Enter selection, when done press <enter> > ");
    return get_input_digit();
}

static void main_loop(void)
{
    int32_t running = 1;

    while (running != 0) {
        int32_t i = print_menu_get_action();
        int32_t res;

        switch (i) {
        case 1:
            res = add_timer();
            if (ERROR_CODE == res) {
                (void)print_string("\nError ... timer not added!\n");
            } else {
                (void)print_string("\nTimer added!\n");
            }
            break;
        case 2:
            (void)print_string("Which timer should I nuke? > ");
            delete_timer_record(get_input_digit());
            break;
        case 3:
            list_timers();
            break;
        case 4:
            display_time();
            break;
        case 9:
            /* Exit */
            (void)print_string("\nGoodbye\n\n");
            running = 0;
            break;
        default:
            /* do nothing ... reprint menu */
            (void)print_string("\nInvalid Action ... try again!\n");
            break;
        }
    }
}

int32_t main(void)
{
    init_timer();     /* setup */
    main_loop();      /* loop until user quits */
    uninit_timer();   /* tear down */
    return 0;
}

