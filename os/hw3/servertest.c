/***********  serverTest.c **************
 *
 * makes 2 fifos named FIFO_to_server and FIFO_to_client
 * opens FIFO_to_server for reading and FIFO_to_client for writing
 *
 ***************************************/

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
    int fda;    // to read from client
    int fdb;    // to write to client
    int finish; // lets me know that client is done

    int readint; // buffer holds a character
    int writeint;  // server puts string here

    /* Create the fifos and open them  */
    if ((mkfifo("FIFO_to_server", 0666) < 0 && errno != EEXIST)) {
        perror("cant create FIFO_to_server");
        return -1;
    }

    if ((mkfifo("FIFO_to_client", 0666) < 0 && errno != EEXIST)) {
        perror("cant create FIFO_to_client");
        return -1;
    }

    if ((fda = open("FIFO_to_server", O_RDONLY)) < 0)
        printf("cant open fifo to write");

    if ((fdb = open("FIFO_to_client", O_WRONLY)) < 0)
        printf("cant open fifo to read");

    finish = read(fda, readint, sizeof(int)); // read the character

    printf("Server: just got int: %d\n", readint);

    writeint = readint*10;
    printf("\nServer: writeint is,%d", writeint);

    write(fdb, writeint, 2);
    printf("\nServer: sent the message");

    if (finish == 1)
        printf("\nServer: This says I am ready to close \n");

    close(fda);
    close(fdb);
    unlink("FIFO_to_server");
    unlink("FIFO_to_client");

    return 0;
}

