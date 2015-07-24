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

int main()
{ 
	

struct  sockaddr_in sad; /* structure to hold server's address  */
  struct  sockaddr_in cad; /* structure to hold client's address  */
   int sd,cb;         /* socket descriptors                  */
  struct hostent *hptr;
socklen_t  addrlen; 

                 
  
 
 
 /* Set up address for local socket */
	memset((char *)&sad,0,sizeof(sad)); /* clear sockaddr structure */
	sad.sin_family = AF_INET;           /* set family to Internet     */
	sad.sin_addr.s_addr = INADDR_ANY;   /* set the local IP address   */
	sad.sin_port = htons((u_short)PORTNUMBER);/* convert to network byte order */
	
	/* Create a socket */
	//int socket(int domain, int type, int protocol);
sd=socket(AF_INET,SOCK_STREAM,0);
        if (sd < 0) {
        perror("socket creation");
        exit(1);
        }


 
 /* Bind a local address to the socket */
	//int bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen);
	if (bind(sd, (struct sockaddr *)&sad, sizeof(struct sockaddr)) < 0) {
        perror("bind");
        exit(1);
	}
 
    if(listen(sd,BSIZE)<0)
    { 
        perror("listen error!"); 
         exit(1); 
    } 
 
          addrlen=sizeof(struct sockaddr_in);
          cb=accept(sd,(struct sockaddr *)&cad,&addrlen);
    while(1) 
    { 
  
        

			         /*  Display a message showing the client's address */
                printf("%d bytes received from ", cb);
                if((hptr = gethostbyaddr((char*)&cad.sin_addr, sizeof(struct sockaddr_in),AF_INET))==0){
                        printf("%s port %d\n", inet_ntoa(cad.sin_addr),ntohs(cad.sin_port));
                }
                else {
                        printf("%s/%s port %d\n", hptr->h_name,inet_ntoa(cad.sin_addr),ntohs(cad.sin_port));
                }
                
                 /*  Send it back to the client */
                 char *recvm="test ";
                 int byss, len, bytesrecv;
                 bytesrecv= recv(cb,recvm, len, 0);
                 len=strlen(recvm);
                byss=send(cb,recvm, len, 0);
        if(byss<0)
        {
            perror("send error!");
            shutdown(cb,2);
            exit(0);
        }
        else
        {
         printf("Send message %s Byte %d ",recvm,byss);
        }
         shutdown(cb, 2);
             
       
}
			 
       
            ;
      } 
   

