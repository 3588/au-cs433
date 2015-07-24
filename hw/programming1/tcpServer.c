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
    int sd,cb; 
    struct sockaddr_in sad; 
    struct sockaddr_in cad; 
    socklen_t  addrlen; 

    if((sd=socket(AF_INET,SOCK_STREAM,0))==-1) 
    { 
        perror("socket creat failed!"); 
         exit(1); 
    } 
 
    sad.sin_family=AF_INET; 
    sad.sin_port=htons(PORTNUMBER); 
    sad.sin_addr.s_addr=INADDR_ANY; 
    bzero(&(sad.sin_zero),8); 
 
    if(bind(sd,(struct sockaddr *)&sad,sizeof(struct sockaddr))==-1) 
    { 
        perror("bind error!"); 
         exit(1); 
    } 
 
    if(listen(sd,BSIZE)==-1) 
    { 
        perror("listen error!"); 
         exit(1); 
    } 
 
    while(1) 
    { 
        addrlen=sizeof(struct sockaddr_in); 
         cb=accept(sd,(struct sockaddr *)&cad,&addrlen); 
         if(1) 
         { 
             char *backmsg="send back test"; 
             int len,bysent; 
             len=strlen(backmsg); 
             bysent=send(cb,backmsg,len,0); 
             if(bysent==-1) 
            { 
                perror("send error!"); 
                 shutdown(cb,2); 
                 exit(0); 
             } 
             else 
             { 
                 printf("Send message %s/nByte %d/n",backmsg,bysent); 
            } 
        }     
      } 
     shutdown(cb,2); 
}
