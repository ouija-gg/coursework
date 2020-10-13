/***************************   clientTest.c   ************************
/
/ opens FIFO_to_server for writing and FIFO_to_client for reading
/
*********************************************************************/

#include <fcntl.h>
#include <stdio.h>
#include <string.h>

main(void) {
    int fda;          // to write to server
    int fdb;          // to read response from server
    char charbuff[1]; // buffer holds a character
    char outchar[2];  // server puts string here

    memset(charbuff, 0, 1);
    memset(outchar, 0, 1);

    if ((fda = open("FIFO_to_server", O_WRONLY)) < 0)
        printf("cant open fifo to write");

    if ((fdb = open("FIFO_to_client", O_RDONLY)) < 0)
        printf("cant open fifo to read");

    printf("Client: Please enter a number: ");
    scanf("%c", &charbuff);

    write(fda, charbuff, 1);
    printf("\nClient: Got the number sent, now waiting for response ");
    read(fdb, outchar, 2);
    printf("\nClient: received from server %d", outchar[0]);

    close(fda);
    close(fdb);

    printf("\nall done!\n");
}

