
/* FTP client that is able to perform the following commands
 * quit:	quit ftp client
 * pwd:		display the server's current working directory
 * ls:		list the directory on the server side
 * get filename: get a file from the server (use passive mode)
 * put filename: upload a file to the server (use passive mode)
 * passive:	enter passive mode
 * active: enter active mode
 * 
 */


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <math.h>

#define SERVER_PORT_ID 433
#define CLIENT_PORT_ID 1433 // client port. not necessary but I just put it there
#define MAXSIZE 512
#define USER "USER CS433FTP\x0d\x0a"
#define PASS "PASS mathcs\x0d\x0a"
#define PASV "PASV\x0d\x0a"
#define PWD "PWD\x0d\x0a"
#define QUIT "QUIT\x0d\x0a"

int comm2Server(int *sd,char *ptr,int size); /* a function for simple communication with the server*/
int getNumber(char * recvline); /* a function to calculate the port number*/


int main(int argc,char *argv[])

{
	/* Variable Declarations*/
	
    int sockid, newsockid,i,len, num_blks;
    struct sockaddr_in my_addr, server_addr; 
    int n,j,k;
    FILE *fp; 
    char in_buf[MAXSIZE];/* a pointer to test the user input for get, put or sth unsupported*/ 
    char fileName[50]; /* assume the length of a file name is less than 50 characters;*/
    int PASVportNumber; // integer to store the port number from PASV
    char *host; /*hold the ascii ip of the server*/
    char userCommand[100]; /* a pointer to hold the client command. Maximum characters: 100*/
    int flag=1; /* flag to quit the ftp client*/
	char recvline[MAXSIZE]; // a char array to store responses from the server
    
    
    /*creating socket*/
	
	 
    /* Bind to local addressr*/
    
 

	/*Connect to the server*/

/******************************************************************************
 * Begin the login process
 * 1. Display the server's welcome message
 * 2. Send in username
 * 3. Send in password
*******************************************************************************/

	/* Display server welcome message 220*/

	/*send user name to the server*/


	/*send the password to the server*/
	
/*****************************************************************************
 * begin to read in user commands
 * passive, pwd, and quit are all simple communications with the server
 * 1. write the command to server
 * 2. display the server's response
 * 
 * get and put are different
 * 1. get filename will require the following steps
 * a). set up the passive environment
 * b). calculate the port number based on the server's response to passive
 * c). set up a new data connection with the server on the new data port
 * d). read in the data from the server and write it to the local file
 * e). close the data connection
 * f). display the server's response about the data transfer
 * 
 * 2. put filename will require the following steps
 * a). set up the passive environment
 * b). calculate the port number based on the server's response to passive
 * c). set up a new data connection with the server on the new data port
 * d). read in the data from local file and write it to the server
 * e). close the data connection
 * f). display the server's response about the data transfer
 * 
 * I will leave the ls and active command for you to figure out by yourself.
 * *******************************************************************************/
	while(flag!=0){

		/*****************************************************************************
		PWD Command
		*****************************************************************************/


		/*****************************************************************************
		active Command
		*****************************************************************************/
		
		
		
		/*****************************************************************************
		passive Command
		*****************************************************************************/



		/*****************************************************************************
		quit Command
		*****************************************************************************/


		/*****************************************************************************
		 ls command
		 *****************************************************************************/
		
		

		/*****************************************************************************
		get or put file
		*****************************************************************************/




				/*****************************************************************************
				get file
				*****************************************************************************/

					/*Enter passive mode*/
					
					/* extract the port number from the message*/
					
					/* connect to the server on the specific port for data transfer*/
					
					
					/* write RETR fileName to the server and get response*/
					
					
					/* start to read in the data*/
					
					
					/*write the data to the local file*/
					
					/* Read in server response and display it*/
					
				/*****************************************************************************
				put file
				*****************************************************************************/
				
					/*use passive mode*/
					
					
					/* extract the port number from the message*/
					
					/* connect to the server on the specific port for data transfer*/
					
					/* write STOR fileName to the server and get response*/
					
					/* start to write the data to the server*/
				
					// obtain file size:
				
					/*read in the data to buffer*/
					
					/* Read in server response and display it*/
					
					

				/*****************************************************************************
				command not supported
				*****************************************************************************/
				
	}
	
	return 0;
}

// Code to communication with server on simple commands. *sd is the socket address
// *ptr points to the message received and size is the number of bytes in the *ptr
/************* this method is only used to communicate simple commands with the server *******
 * You don't have to use this method but it gives you an idea of how the communication works between
 * the client and server ***************/
int comm2Server (int * sd, char * ptr, int size)
{
	char recvline[MAXSIZE];
	int i,n;
	/* WRITE TO COMMAND TO THE SERVER*/
	if(write(*sd,ptr,strlen(ptr))<0){
		printf("write error\n");
		return 0;
	}
	printf("Write to server: %s\n", ptr);
		
	/* DISPLAY THE SERVER RESPONSE*/
	if((n=read(*sd,recvline,MAXSIZE))<0){
		printf("receive error\n");
		return 0;
	}
	printf("Server Response:",n);
	for (i=0;i<n;i++){
		printf("%c",recvline[i]);
	}    
	return 1;
}
// get the port number from PASV. You should use this function
// to convert the text port number into an int
int getNumber(char * recvline)
{
	char * ch;
	char temp[10];// a pointer to hold number in ASCII format
	int num, i=0,len,j=0,k;
	double finalNumber=0;
	/* get port number*/

	num=4;
	ch=strchr(recvline,'(');
	while(num!=0){
		/*printf("%c",ch[i]);*/
		if(ch[i]==','){
			num--;
		}
		i++;
	}
	// now we get the port numbers
	for(num=0;num<2;num++){
		j=0;
		while(ch[i]>='0' && ch[i]<='9'){
			temp[j]=ch[i];
			j++;
			i++;
		}
		len=j;
		for(k=0;k<len;k++){
			//printf("%c",temp[k]);
			finalNumber=(double)(temp[k]-48)*pow(10,j-1)+finalNumber;
			j--;
		}
		if(num==0){
			finalNumber=finalNumber*256;
		}
		i++;
	}

	return (int)(finalNumber);
}
	
