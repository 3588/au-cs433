/*code for example echo server program that uses TCP */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define PORTNUMBER      6000            /* default protocol port number */
#define QLEN            10               /* size of request queue        */
#define BSIZE           1000            /* size of data buffer          */

/*------------------------------------------------------------------------
 * Program:   tcp echo server
 *
 * Purpose:   allocate a socket and then repeatedly execute the following:
 *              (1) wait for the next connection from a client
 *              (2) loop:
 *                      read data from the socket
 *                      write the data back to the socket
 *                  until the connection is closed by the client
 *              (3) go back to step (1)
 *
 * Syntax:    tcpServer
 *
 * You can follow the hints below to finish the code step by step.
 *------------------------------------------------------------------------
 */
main(int argc, char**argv)
{
  struct  sockaddr_in sad; /* structure to hold server's address  */
  struct  sockaddr_in cad; /* structure to hold client's address  */
  int     sd, sd2;         /* socket descriptors                  */
  int     alen;            /* length of address                   */
  int     bytect;          /* byte count for the current client   */
  int     n;               /* byte count for the current request  */
  char    buf[BSIZE];      /* buffer for string the server sends  */
  struct hostent *hptr;

  /* Initialize the server address */


  /* Create a socket */


  /* Bind a local address to the socket */

 
  /* Make the socket a listening socket */


  /* Main server loop - accept and handle requests */

  while (1) {
	  //do stuff

  }
		

}

