/* This programming assignment has three parts.
 1. the examination of UDP communcations --> 20 points
 2. the coding for tcpServer --> 60 points
	functionality: 40 points
	I/O interface: 15 points
	readability: 5 points
 3. the examination of TCP communications --> 20 points
 
 Total: 100 points
 */


/* Solution to UDP packet analysis
 Your answer may be different based on differnt inputs you have. In the packets for this solution, 
 the answers are
 1. There are 6 packets captured by the filter
 2. 3 packets are from my local machine to the server and 3 packets back. (by looking at the source
    and destination of packets)
 3. local machine's ethernet address: 00:19:07:ef:84:00 (can be found in the ethernet protocol)
	remote machine's ethernet address: 00:04:4b:15:6f:e5
 4. I chose #1 packet
	a). I typed hello (can be found in the data section of the packet)
	b). the length of applicaition data is 7
	c). the UDP header length is 8 bytes by adding up all the segments in the header
*/


/* Solution to the TCP packet analysis
 Again, your answer may vary from this because of different inputs.
 1. 16 packets were captured by the filter
 2. 6 are from my local machine and 10 are from the server
 3. The first three packets are used to set up the connection (three way hand shake)
    the last three were used to stop the connection (in theory, there should be four. But
	in reality, piggybacking is used to combine FIN and ACK together)
	Out of the 16 total packets, 6 of them are purely ACK packets.
 4. local machine's ethernet address: 00:19:07:ef:84:00 (can be found in the ethernet protocol)
	remote machine's ethernet address: 00:04:4b:15:6f:e5
 5. I chose the #4 packet
	a) I typed hello
	b) the length of my text is 7 byes
	c) 32 bytes are in the TCP header (similar to UDP)
	d) 20 bytes are in the IP header (similar to adding everything up in IP section)
	e) 14 bytes are in the ehternet header (by counting the source, destination and type)
	f) total is 73 bytes
 */
 



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
 * Program:   echo server
 *
 * Purpose:   allocate a socket and then repeatedly execute the following:
 *              (1) wait for the next connection from a client
 *              (2) loop:
 *                      read data from the socket
 *                      write the data back to the socket
 *                  until the connection is closed by the client
 *              (3) go back to step (1)
 *
 * Syntax:    eserver
 *
 *------------------------------------------------------------------------
 */
int main(int argc, char**argv) {
	struct  sockaddr_in sad; /* structure to hold server's address  */
	struct  sockaddr_in cad; /* structure to hold client's address  */
	int     sd, sd2;         /* socket descriptors                  */
	int     alen;            /* length of address                   */
	int     bytect;          /* byte count for the current client   */
	int     n;               /* byte count for the current request  */
	char    buf[BSIZE];      /* buffer for string the server sends  */
	struct hostent *hptr;
	
	/* Initialize the server address */
	memset(&sad.sin_zero,0,sizeof(sad.sin_zero));
	sad.sin_family=AF_INET;
	sad.sin_port=htons(PORTNUMBER);
	sad.sin_addr.s_addr=INADDR_ANY;
	
	/* Create a socket */
	sd=socket(PF_INET,SOCK_STREAM,0);
	if(sd<0){
		perror("socket");
		exit(1);
	}
	/* Bind a local address to the socket */
	if(bind(sd, (struct sockaddr *)&sad, sizeof(sad))<0) {
		perror("bind");
		exit(1);
	}
	
	/* Make the socket a listening socket */
	if(listen(sd,QLEN)<0){
		perror("listen");
		exit(1);
	}
	/* Main server loop - accept and handle requests */
	while (1) {
		alen=sizeof(cad);
		if ( (sd2=accept(sd, (struct sockaddr *)&cad, &alen))<0) {
			perror("accept");
			exit(1);
		}
		if((hptr = gethostbyaddr((char*)&cad.sin_addr,sizeof(struct sockaddr_in),AF_INET))==0)
			printf("Connection accepted from %s port %d\n",inet_ntoa(cad.sin_addr),ntohs(cad.sin_port));
		else
			printf("connection made with %s %s on port %d\n",hptr->h_name,inet_ntoa(cad.sin_addr),ntohs(cad.sin_port));
		bytect=0;
		while((n=read(sd2,buf,BSIZE))>0){
			write(sd2,buf,n);
			bytect+=n;
		}
		printf("Total number of bytes echoed: %d\n", bytect);
		close(sd2);
	}
	close(sd);
	
	return 0;
}


