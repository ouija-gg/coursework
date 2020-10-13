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
    int readint; // buffer holds a character
    int writeint;  // server puts string here

    if ((fda = open("FIFO_to_server", O_WRONLY)) < 0)
        printf("cant open fifo to write");

    if ((fdb = open("FIFO_to_client", O_RDONLY)) < 0)
        printf("cant open fifo to read");

    // get the number to send to server
    printf("Client: Please enter a number: ");
    scanf("%d[\n]", &readint);
    printf("got :%d\n", readint);

    write(fda, readint, sizeof(int));
    printf("\nClient: sent message, now waiting for response ");
    read(fdb, writeint, sizeof(int));
    printf("\nClient: received from server %d", writeint);

    close(fda);
    close(fdb);

    printf("\nall done!\n");
}

