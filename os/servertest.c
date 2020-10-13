/*********************************   serverTest.c   *****************************
* 
* makes 2 fifos named FIFO_to_server and FIFO_to_client
* opens FIFO_to_server for reading and FIFO_to_client for writing
*
**********************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

main (void)
{
  int fda;	// to read from client
  int fdb;	// to write to client
  int finish;	// lets me know that client is done
  int i;	// because C needs this defined as int

  char charbuff[1];	// buffer holds a character
  char outchar[7];	// server puts string here
  
  memset(charbuff,0,1); 
  memset(outchar,0,7); 


  /* Create the fifos and open them  */
  if ((mkfifo("FIFO_to_server",0666)<0 && errno != EEXIST))
	{
	perror("cant create FIFO_to_server");
	exit(-1);
	}

   if ((mkfifo("FIFO_to_client",0666)<0 && errno != EEXIST))
	{
	perror("cant create FIFO_to_client");
	exit(-1);
	}

  if((fda=open("FIFO_to_server", O_RDONLY))<0)
     printf("cant open fifo to write");

  if((fdb=open("FIFO_to_client", O_WRONLY))<0)
     printf("cant open fifo to read");
  
  finish=read(fda, charbuff, 1); 	//read the character
  
  printf("Server: just got character: ,%c", charbuff[0]);

  for( i = 0; i<5; i++)
    outchar[i] = '*';
  outchar[5] = charbuff[0];
  outchar[6] = 0;
  printf("\nServer: outchar is,%s", outchar);

  write(fdb, outchar, 7); 
  printf("\nServer: Got the characters sent");
 
  if(finish == 1)
    printf("\nServer: This says I am ready to close \n");

  close(fda);
  close(fdb);
  unlink("FIFO_to_server");
  unlink("FIFO_to_client");

  

}



