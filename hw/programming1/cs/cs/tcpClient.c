/* code for example echo client program that uses TCP */

#include <sys/types.h> 
#include <sys/socket.h> // together with types.h, provide functions for socket(),bind(),listen() and accept()
#include <netinet/in.h> // defines internet address struct
#include <arpa/inet.h> //inet_aton() and inet_ntoa();
#include <netdb.h>// gethostbyname() and gethostbyaddr()

#include <stdio.h>
#include <string.h> // for perror() and I/O

#define PORTNUMBER       6000           /* default protocol port number */
#define MAXLINE          1000           /* length of line buffer        */


char    localhost[] =   "localhost";    /* default host name            */

/*------------------------------------------------------------------------
 * Program:   tcp echo client
 *
 * Purpose:   allocate a socket, connect to an echo server.  Then accept
 *            keyboard input and send it to the server.  When a response is
 *            received, display it at the user terminal.
 *
 * Syntax:    tcpClient [ host ]
 *
 *            host  - name of a computer on which server is executing.  If
 *                    missing, use local host.
 *
 *------------------------------------------------------------------------
 */
main(int argc, char**argv){
	struct  hostent  *ptrh;  /* pointer to a host table entry       */
	struct  sockaddr_in sad; /* structure to hold an IP address     */
	int     sd;              /* socket descriptor                   */
	char    *host;           /* pointer to host name                */
	char    sendline[MAXLINE]; /* send buffer                       */
	char    recvline[MAXLINE]; /* receive buffer                    */
	struct hostent *hptr;


	 struct sockaddr_in {
    
    	short            sin_family;  // e.g. AF_INET
    	unsigned short   sin_port;    // e.g. htons(3490)
    	struct in_addr   sin_addr;    // see struct in_addr, below
    	char             sin_zero[8]; // zero this if you want to
	};
	
	struct in_addr {
    	unsigned long s_addr;
	};
	
  
	/* Set up the server address/port */
	memset((char *)&sad.sin_zero,0,sizeof(sad.sin_zero)); /* clear sockaddr structure */
	sad.sin_family = AF_INET;         /* set family to Internet     */
	sad.sin_port = htons((u_short)PORTNUMBER); /* convert port number to network byte order */

	/* Check host argument and assign host name. */

	if (argc > 1) {
		host = argv[1];         /* if host argument specified   */
	} 
	else {
        host = localhost;
	}

	/* Convert host name to equivalent IP address and copy to sad. */
  
	//struct hostent *gethostbyname(const char *name);
	//struct hostent *gethostbyaddr(const char *addr, int len, int type);
    
	if((ptrh=gethostbyname(host))==0){
        fprintf(stderr,"invalid host: %s\n", host);
        exit(1);
	}
	memcpy(&sad.sin_addr, ptrh->h_addr, ptrh->h_length);

	/* Create a socket. */
	//int socket(int domain, int type, int protocol);
	sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sd < 0) {
        perror("socket creation");
        exit(1);
	}

	/* Connect the socket to the specified server. */
	//int connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen);

	if (connect(sd, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
        perror("connect");
        exit(1);
	}

	if((hptr=gethostbyaddr((char*)&sad.sin_addr,sizeof(struct sockaddr_in),AF_INET))==0)
		printf("Connection made to %s port %d\n",inet_ntoa(sad.sin_addr),ntohs(sad.sin_port));
	else
		printf("Connection made to %s/%s port %d\n",hptr->h_name,inet_ntoa(sad.sin_addr),ntohs(sad.sin_port));


	/* Repeatedly get data from stdin, write it to the socket, 
     then read data from socket and write it to stdout        */

	int repeat=1;
	while (repeat && (fgets(sendline,MAXLINE,stdin) != NULL)) {
		write(sd,sendline,1+strlen(sendline));
		if(read(sd,recvline,1+strlen(sendline))==0){
			printf("server disconnected\n");
			repeat=0;
		}
		else {
			fputs(recvline,stdout);
		}

	}
	/* Close the socket. */
	close(sd);
	/* Terminate the client program gracefully. */
	exit(0);
}

