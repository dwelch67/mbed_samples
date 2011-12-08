
#include "common.h"

static unsigned int td_tick_last;
static unsigned int td_send_char;
static unsigned int td_char_to_send;
static unsigned int td_led_state;

unsigned int threadd ( unsigned int event )
{
    unsigned int nowtick;
    unsigned int tickdiff;

    switch(event)
    {
        case INIT:
        {
            td_send_char=1;
            td_char_to_send=0x34;
            td_tick_last=get_timer_tick();
            td_led_state=0;
            change_led_state(3,td_led_state);
            return(0);
        }
        case HEARTBEAT:
        {
            if(td_send_char)
            {
                if(uart_tx_if_ready(td_char_to_send)) td_send_char=0;
            }
            nowtick=get_timer_tick();
            tickdiff=(nowtick-td_tick_last)&TIMER_TICK_MASK;
            if(tickdiff>=7000000)
            {
                td_tick_last=(td_tick_last+7000000)&TIMER_TICK_MASK;
                td_send_char=1;
                //td_char_to_send=
                td_led_state=(td_led_state+1)&1;
                change_led_state(3,td_led_state);
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
