/*code for example echo server program that uses UDP */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <stdio.h>
#include <string.h>

#define PORTNUMBER      6001            /* default protocol port number */
#define BSIZE           1000            /* size of data buffer          */

/*------------------------------------------------------------------------
 * Program:   udp echo server
 *
 * Purpose:   allocate a datagram socket and then repeatedly 
 *            execute the following:
 *                read data from the socket
 *                write the data back to the socket
 *
 * Syntax:    udpServer
 *
 *------------------------------------------------------------------------
 */
main(int argc, char**argv){
    struct  sockaddr_in sad; /* structure to hold server's address  */
    struct  sockaddr_in cad; /* structure to hold client's address  */
    int     fromlen = sizeof(cad);
    int     sd;              /* socket descriptor                   */
    int     n;               /* byte count for the current request  */
    char    buf[BSIZE];      /* buffer for string the server sends  */
    struct hostent *hptr;
	/* struct sockaddr_in {
    
    	short            sin_family;  // e.g. AF_INET
    	unsigned short   sin_port;    // e.g. htons(3490)
    	struct in_addr   sin_addr;    // see struct in_addr, below
    	char             sin_zero[8]; // zero this if you want to
	};
	
	struct in_addr {
    	unsigned long s_addr;
	};
	*/
	
	/* Set up address for local socket */
	memset((char *)&sad,0,sizeof(sad)); /* clear sockaddr structure */
	sad.sin_family = AF_INET;           /* set family to Internet     */
	sad.sin_addr.s_addr = INADDR_ANY;   /* set the local IP address   */
	sad.sin_port = htons((u_short)PORTNUMBER);/* convert to network byte order */

	/* Create a socket */
	//int socket(int domain, int type, int protocol);
	sd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sd < 0) {
		perror("socket creation");
        exit(1);
	}

	/* Bind a local address to the socket */
	//int bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen);
	if (bind(sd, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
        perror("bind");
        exit(1);
	}

	/* Main server loop - receive and handle requests */

	while (1) {
		/*
		 ssize_t recv(int s, void *buf, size_t len, int flags);
		 ssize_t recvfrom(int s, void *buf, size_t len, int flags,
         struct sockaddr *from, socklen_t *fromlen);*/

        /* Inner loop.  Read and echo data received from client. */
        n = recvfrom(sd,buf,BSIZE,0,(struct sockaddr *)&cad, &fromlen);
		if(n<0){
			perror("recvfrom");
			exit(1);
		}

		/*  Display a message showing the client's address */

		printf("%d bytes received from ", n);
		if((hptr = gethostbyaddr((char*)&cad.sin_addr, sizeof(struct sockaddr_in),AF_INET))==0){
			printf("%s port %d\n", inet_ntoa(cad.sin_addr),ntohs(cad.sin_port));
		}
		else {
			printf("%s/%s port %d\n", hptr->h_name,inet_ntoa(cad.sin_addr),ntohs(cad.sin_port));
		}
		/* ssize_t send(int s, const void *buf, size_t len, int flags);
		 ssize_t sendto(int s, const void *buf, size_t len,
               int flags, const struct sockaddr *to,
               socklen_t tolen);
		 */
		/*  Send it back to the client */
        sendto(sd,buf,n,0,(struct sockaddr *)&cad,fromlen);

	}
}

