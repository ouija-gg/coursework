/*****************   firstTest.c  ********************
*
*  Purpose: This program gets a character from the 
*           keyboard and prints it
*
*****************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <fcntl.h>

int main (void)
{

    char charbuffer[1];	// buffer holds a character
    int intbuffer[1];
    char outchar[2];	// put the string here

    printf("Enter a number: ");
    scanf("%d%*c", &intbuffer);
    printf("Enter a character: ");
    scanf("%c", &charbuffer);
    outchar[0] = charbuffer[0];
    outchar[1] = 0;
    int outint[2] = { intbuffer[0], 0 };

    printf("\nOutput: the character is %s%s%s%s%s", outchar,outchar,outchar,outchar,outchar);
    printf("\nOutput of int times 5: %d * 5 = %d", outint[0], outint[0]*5);

    printf ("\nall done!\n");
    return 0;

}


