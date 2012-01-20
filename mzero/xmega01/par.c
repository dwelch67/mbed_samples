
#include <stdio.h>


int main ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    for(ra=0x00;ra<=0xFF;ra++)
    {
        rc=0;
        for(rb=0x80;rb;rb>>=1) if(ra&rb) rc++;
        rc&=1;
        printf("  %u, //0x%02X\n",rc,ra);
    }

    return(0);
}


