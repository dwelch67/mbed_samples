
#include "common.h"

#define MY_LED 1

unsigned int threadb ( unsigned int event )
{
static unsigned int led_state;
static unsigned int timer_count;

    switch(event)
    {
        case INIT:
        {
            led_state=0;
            change_led_state(MY_LED,led_state);
            return(0);
        }
        case LOOP:
        {
            return(0);
        }
        case TIMER:
        {
            timer_count=4;
            return(0);
        }
        case HEARTBEAT:
        {
            if(timer_count)
            {
                timer_count--;
                led_state=(led_state+1)&1;
                change_led_state(MY_LED,led_state);
            }
            return(0);
        }
        default:
        {
            break;
        }
    }
    return(0);
}
