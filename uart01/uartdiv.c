

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int pclk;
unsigned int dbaud;
unsigned int baud;
unsigned int nmul;
unsigned int ndiv;
unsigned int dl;

unsigned int newdiff;
unsigned int mindiff;

double da;
double db;


int main ( void )
{
    pclk=1000000;
    dbaud=9600;

    mindiff=0; mindiff--;

    for(dl=1;dl<=0xFFFF;dl++)
    {
        for(ndiv=0;ndiv<15;ndiv++)
        {
            for(nmul=ndiv+1;nmul<=15;nmul++)
            {
                da=pclk;
                da/=16;
                da/=dl;
                db=ndiv;
                db/=nmul;
                db+=1;
                da/=db;
                da+=0.5;
                baud=da;
                if(baud>dbaud) newdiff=baud-dbaud;
                else           newdiff=dbaud-baud;
                if(newdiff<=mindiff)
                {
                    printf("dl 0x%02X mul 0x%02X div 0x%02X baud %u diff %u\n",dl,nmul,ndiv,baud,newdiff);
                    mindiff=newdiff;
                }
            }
        }
    }

    return(0);
}






