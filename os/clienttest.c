/***************************   clientTest.c   ************************
/ 
/ opens FIFO_to_server for writing and FIFO_to_client for reading
/
*********************************************************************/


#include <stdio.h>
#include <fcntl.h>

main (void)
{
  int fda;	// to write to server
  int fdb;	// to read response from server
  char charbuff[1];	// buffer holds a character
  char outchar[7];	// server puts string here
 
  memset(charbuff,0,1); 
  memset(outchar,0,7); 


  if((fda=open("FIFO_to_server", O_WRONLY))<0)
     printf("cant open fifo to write");

  if((fdb=open("FIFO_to_client", O_RDONLY))<0)
     printf("cant open fifo to read");

  printf("Client: Please enter a character: ");
  scanf("%c", &charbuff);


  write(fda, charbuff, 1);
  printf("\nClient: Got the character sent, now waiting for response ");
  read(fdb, outchar, 7);
  printf("\nClient: received from server %s", outchar);

  close(fda);
  close(fdb);

  printf ("\nall done!\n");
   
}


