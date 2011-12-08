
#include "common.h"

static unsigned int tc_tick_last;
static unsigned int tc_send_char;
static unsigned int tc_char_to_send;
static unsigned int tc_led_state;

unsigned int threadc ( unsigned int event )
{
    unsigned int nowtick;
    unsigned int tickdiff;

    switch(event)
    {
        case INIT:
        {
            tc_send_char=1;
            tc_char_to_send=0x33;
            tc_tick_last=get_timer_tick();
            tc_led_state=0;
            change_led_state(2,tc_led_state);
            return(0);
        }
        case HEARTBEAT:
        {
            if(tc_send_char)
            {
                if(uart_tx_if_ready(tc_char_to_send)) tc_send_char=0;
            }
            nowtick=get_timer_tick();
            tickdiff=(nowtick-tc_tick_last)&TIMER_TICK_MASK;
            if(tickdiff>=6000000)
            {
                tc_tick_last=(tc_tick_last+6000000)&TIMER_TICK_MASK;
                tc_send_char=1;
                //tc_char_to_send=
                tc_led_state=(tc_led_state+1)&1;
                change_led_state(2,tc_led_state);
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
