
#define PORTF_BASE 0x6A0
#define PORTF_DIRSET (*((volatile unsigned char *)(PORTF_BASE+0x01)))
#define PORTF_OUT (*((volatile unsigned char *)(PORTF_BASE+0x04)))

void dummy ( void );

//unsigned char fun ( unsigned char a, unsigned char b, unsigned char c, unsigned char d )
//{
    //return(a+b+c+d);
//}

void notmain ( void )
{
    unsigned int ra;
    unsigned char ca;

    PORTF_DIRSET=0x80;
    PORTF_OUT=0x80;

    while(1)
    {
        for(ra=0x3000;ra;ra--) dummy();
        PORTF_OUT^=0x80;
    }

}
