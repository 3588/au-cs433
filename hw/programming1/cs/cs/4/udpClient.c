/*code for example echo client program that uses UDP */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <stdio.h>
#include <string.h>

#define PORTNUMBER       6001           /* default protocol port number */
#define MAXLINE          1000           /* length of line buffer        */

//extern  int             errno;
/*------------------------------------------------------------------------
 * Program:   udp echo client
 *
 * Purpose:   allocate a socket, connect to an echo server.  Then accept
 *            keyboard input and send it to the server.  When a response is
 *            received, display it at the user terminal.
 *
 * Syntax:    udpClient [ host ]
 *
 *            host  - name of a computer on which server is executing.  If
 *                    missing, use local host.
 *
 *------------------------------------------------------------------------
 */
main(int argc, char**argv){
    struct  hostent  *ptrh;  /* pointer to a host table entry       */
    struct  sockaddr_in sad; /* structure to hold an IP address     */
    struct  sockaddr_in cad; /* structure to hold an IP address     */
    int     sd;              /* socket descriptor                   */
    int     fromlen = sizeof(sad);
    int     nbytes;          /* number of bytes in reply message    */
    char    *host;           /* pointer to host name                */
    char    sendline[MAXLINE]; /* send buffer                       */
    char    recvline[MAXLINE]; /* receive buffer                    */

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

	/*  Set up address for echo server  */
	memset((char *)&sad,0,sizeof(sad)); /* clear sockaddr structure */
	sad.sin_family = AF_INET;         /* set family to Internet     */
	sad.sin_port = htons((u_short)PORTNUMBER); /* convert port number to network byte order */

	/*  Set up address for local socket  */

	memset((char *)&cad,0,sizeof(sad)); /* clear sockaddr structure */
	cad.sin_family = AF_INET;         /* set family to Internet     */
	cad.sin_addr.s_addr = INADDR_ANY; /* set the local IP address   */
	cad.sin_port = 0;  /* use any available port */

	/* Check host argument and assign host name. */

	if (argc > 1) {
        host = argv[1];         /* if host argument specified   */
	} 
	else {
        host = "localhost";
	}


	/* Convert host name to equivalent IP address and copy to sad. */
	//struct hostent *gethostbyname(const char *name);
	//struct hostent *gethostbyaddr(const char *addr, int len, int type);
	if((ptrh=gethostbyname(host))==0){
        fprintf(stderr,"invalid host: %s\n", host);
        exit(1);
	}
	memcpy(&sad.sin_addr, ptrh->h_addr, ptrh->h_length);

	/* Create a datagram socket */

	sd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sd < 0) {
        perror("socket creation");
        exit(1);
	}

	/* Bind the socket to any local address */
	//int bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen);
	if(bind(sd, (struct sockaddr *)&cad, sizeof(cad))<0){
		perror("bind");
		exit(1);
	}

	/* Main loop.  Repeatedly get data from stdin, write it to
     the socket, then read data from socket and write it to stdout */

	while (fgets(sendline,MAXLINE,stdin) != NULL) {

		/*  Send a message to the server  */
		/* ssize_t send(int s, const void *buf, size_t len, int flags);
		 ssize_t sendto(int s, const void *buf, size_t len,
               int flags, const struct sockaddr *to,
               socklen_t tolen);
		 */
		if(sendto(sd,sendline,1+strlen(sendline),0, (struct sockaddr*)&sad, sizeof(sad))<0){
			perror("sendto");
			exit(1);
		}

		/*  Wait for a reply  */
		/*
		 ssize_t recv(int s, void *buf, size_t len, int flags);
		 ssize_t recvfrom(int s, void *buf, size_t len, int flags,
                 struct sockaddr *from, socklen_t *fromlen);*/
		if((nbytes=recvfrom(sd,recvline,1+strlen(sendline),0, (struct sockaddr*)&sad, &fromlen))<0){
			perror("recvfrom");
			exit(1);
		}

		printf("Received %d bytes: %s", nbytes, recvline);
	}

	/* Close the socket. */

	close(sd);
}

