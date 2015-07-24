
/* FTP client that is able to perform the following commands
 * quit:	quit ftp client
 * pwd:		display the server's current working directory
 * ls:		list the directory on the server side
 * get filename: get a file from the server (use passive mode)
 * put filename: upload a file to the server (use passive mode)
 * passive:	enter passive mode
 * active:     enter active mode (tricky. see my explanation for details)
 * compile this code as gcc -o ftp2 ftp.c -lm
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

#define SERVER_PORT_ID 21
#define CLIENT_PORT_ID 1433 // client port. not necessary but I just put it there
#define MAXSIZE 512
#define USER "USER mathcs\x0d\x0a"
#define PASS "PASS aumathcs\x0d\x0a"
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
    
    

  if(argc != 2) {

    printf("error: usage : ./ftp2 server_ip\n"); 

    exit(0);
  }

  host = argv[1];

  len = strlen(argv[1]);
    

  /*creating socket and connect to the server*/
  printf("client: creating socket\n");

  if ((sockid = socket(AF_INET,SOCK_STREAM,0)) < 0){ 

    printf("client: socket error : %d\n", errno); 

    exit(0);


  }
	

  printf("client: binding my local socket\n");

  bzero((char *) &my_addr,sizeof(my_addr));

  my_addr.sin_family = AF_INET;

  my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  my_addr.sin_port = htons(CLIENT_PORT_ID);

  if (bind(sockid ,(struct sockaddr *) &my_addr,sizeof(my_addr)) < 0){

    printf("client: bind  error :%d\n", errno); 

    exit(0);


  }
	

  printf("client: starting connect\n");

  bzero((char *) &server_addr,sizeof(server_addr));

  server_addr.sin_family = AF_INET;

  server_addr.sin_addr.s_addr = inet_addr(host);

  server_addr.sin_port = htons(SERVER_PORT_ID);

  if (connect(sockid ,(struct sockaddr *) &server_addr,sizeof(server_addr)) < 0){

    printf("client: connect  error :%d\n", errno);

    exit(0);

  }

  /******************************************************************************

   * Begin the login process
   * 1. Display the server's welcome message
   * 2. Send in username
   * 3. Send in password
  *******************************************************************************/


  /* Display server welcome message 220*/

  if((n=read(sockid,recvline,MAXSIZE))<0){

      printf("receive error\n");

      exit(1);

    }

  printf("Server Response:",n);

  for (i=0;i<n;i++){

    printf("%c",recvline[i]);

  }    
	

  /*send user name to the server*/

  if(comm2Server(&sockid,USER,strlen(USER))==0){

    printf("\nCommunication Error!\n");

    exit(1);

  }
	

  /*send the password to the server*/

  if(comm2Server(&sockid,PASS,strlen(PASS))==0){

    printf("\nCommunication Error!\n");

    exit(1);

  }
	

  /*****************************************************************************
  * begin to read in user commands
  * passive,  pwd, and quit are all simple communications with the server
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
  * ls is similar to get and put except there is no file operations
  * *******************************************************************************/

  while(flag!=0){

    printf("ftp>");

    gets(userCommand);

    /*****************************************************************************

	PWD Command

    *****************************************************************************/

    if((strcmp(userCommand,"PWD")==0) || (strcmp(userCommand,"pwd")==0) ){

	if(comm2Server(&sockid,PWD,strlen(PWD))==0){

	  printf("\nCommunication Error!\n");

	  exit(1);

	}

      }

    /*****************************************************************************

	passive Command

    *****************************************************************************/


    else if((strcmp(userCommand,"passive")==0)||(strcmp(userCommand,"PASSIVE")==0)){

	if(comm2Server(&sockid,PASV,strlen(PASV))==0){

	  printf("\nCommunication Error!\n");

	  exit(1);

	}

      }
		

    /*****************************************************************************

	active Command

    *****************************************************************************/

    else if ((strcmp(userCommand, "active")==0) || (strcmp(userCommand, "ACTIVE")==0)){

      printf("\n Enter Active Mode\n");// this is tricky coz only when you start to transfer data, the active mode makes sense. Since we assume that the passive mode is default, there is no need to actually implement the active transfer protocol.


    }
		

    /*****************************************************************************

	quit Command

    *****************************************************************************/


    else if((strcmp(userCommand,"quit")==0)||(strcmp(userCommand,"QUIT")==0)){

	if(comm2Server(&sockid,QUIT,strlen(QUIT))==0){

	  printf("\nCommunication Error!\n");

	  exit(1);

	}

	close(sockid);

	flag=0;

      }
		

    /*****************************************************************************

	ls command

    *****************************************************************************/


    else if((strcmp(userCommand,"ls")==0)||(strcmp(userCommand,"ls")==0)){


      /*use passive mode*/
      if(write(sockid,PASV,strlen(PASV))<0){

	printf("write error\n");
	exit(1);
      }


      if((n=read(sockid,recvline,MAXSIZE))<0){

	printf("receive error\n");
	exit(1);
      }

      /* extract the port number from the message*/
      PASVportNumber=getNumber(recvline);
				      

      /* connect to the server on the specific port for data transfer*/
      if ((newsockid = socket(AF_INET,SOCK_STREAM,0)) < 0){ 

	printf("client: socket error : %d\n", errno); 

	exit(0);

      }

      server_addr.sin_port = htons(PASVportNumber);

      if (connect(newsockid ,(struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){

	printf("client: connect  error :%d\n", errno); 

	exit(0);

      }

      /* Write List to the server and get response*/
      strcpy(in_buf,"LIST\x0d\x0a");

      len=strlen(in_buf);

      if(write(sockid,in_buf,len)<0){

	printf("write error\n");

	exit(1);

      }

      if((n=read(sockid,recvline,MAXSIZE))<0){

	printf("receive error\n");

	exit(1);

      }


      /* start to read in the data*/
      if((n=read(newsockid,recvline,MAXSIZE))<0){

	printf("receive error\n");

	exit(1);

      }

      //Display the directory listing
      printf("%s", recvline);

      close(newsockid);

      /* Read in server response and display it*/
      if((n=read(sockid,recvline,MAXSIZE))<0){

	printf("receive error\n");

	exit(1);

      }

      printf("Server Response:",n);

      for (i=0;i<n;i++){

	printf("%c",recvline[i]);

      } 		
	  

    }
  

    /*****************************************************************************

	get filename, put filename, or not a supported command

    *****************************************************************************/
	

    else{

      /*****************************************************************************

	get or put file

      *****************************************************************************/
   

      if(strlen(userCommand)>=3){

	strncpy(in_buf,userCommand,3);

	in_buf[3]='\0';


	/*get the file name*/

	j=0;

	for (i=4;i<strlen(userCommand);i++){

	  fileName[j]=userCommand[i];

	  j++;

	}


	/*****************************************************************************

			get file

	*****************************************************************************/

	if((strcmp(in_buf,"get")==0)||(strcmp(in_buf,"GET")==0)){

	  /*use passive mode*/

	  if(write(sockid,PASV,strlen(PASV))<0){

	    printf("write error\n");

	    exit(1);

	  }

	  if((n=read(sockid,recvline,MAXSIZE))<0){

	    printf("receive error\n");

	    exit(1);

	  }

	  

	  /* extract the port number from the message*/
	  PASVportNumber=getNumber(recvline);
					

	  /* connect to the server on the specific port for data transfer*/
	  if ((newsockid = socket(AF_INET,SOCK_STREAM,0)) < 0){ 

	    printf("client: socket error : %d\n", errno); 

	    exit(0);

	  }

	  server_addr.sin_port = htons(PASVportNumber);

	  if (connect(newsockid ,(struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){

	    printf("client: connect  error :%d\n", errno); 

	    exit(0);

	  }

	  
	  /* write RETR fileName to the server and get response*/
	  strcpy(in_buf,"RETR ");

	  strcat(in_buf,fileName);

	  strcat(in_buf,"\x0d\x0a");

	  len=7+strlen(fileName);

	  if(write(sockid,in_buf,len)<0){

	    printf("write error\n");

	    exit(1);

	  }

	  if((n=read(sockid,recvline,MAXSIZE))<0){

	    printf("receive error\n");

	    exit(1);

	  }

	  /* start to read in the data*/

	  if((n=read(newsockid,recvline,MAXSIZE))<0){

	    printf("receive error\n");

	    exit(1);

	  }

	  close(newsockid);

	  /*write the data to the local file*/

	  if((fp=fopen(fileName, "w+"))==NULL){

	    printf("File open error at the client side\n");

	    exit(1);

	  }

	  if(fwrite(recvline,1,n,fp)!=n){

	    printf("File writing error at the client side");

	    exit(1);

	  }

	  fclose(fp);

	  

	  /* Read in server response and display it*/
	  if((n=read(sockid,recvline,MAXSIZE))<0){

	    printf("receive error\n");

	    exit(1);

	  }

	  printf("Server Response:",n);

	  for (i=0;i<n;i++){

	    printf("%c",recvline[i]);

	  } 	      	 

	}

	/*****************************************************************************

			put file

	*****************************************************************************/

	else if((strcmp(in_buf,"put")==0)||(strcmp(in_buf,"PUT")==0)){

	  /*use passive mode*/

	  if(write(sockid,PASV,strlen(PASV))<0){

	    printf("write error\n");

	    exit(1);

	  }

	  if((n=read(sockid,recvline,MAXSIZE))<0){

	    printf("receive error\n");

	    exit(1);

	  }

	  
	  /* extract the port number from the message*/
	  PASVportNumber=getNumber(recvline);

	  

	  /* connect to the server on the specific port for data transfer*/
	  if ((newsockid = socket(AF_INET,SOCK_STREAM,0)) < 0){ 

	    printf("client: socket error : %d\n", errno); 

	    exit(0);

	  }

	  server_addr.sin_port = htons(PASVportNumber);

	  if (connect(newsockid ,(struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){

	    printf("client: connect  error :%d\n", errno); 

	    exit(0);

	  }

	  

	  /* write STOR fileName to the server and get response*/

	  strcpy(in_buf,"STOR ");

	  strcat(in_buf,fileName);

	  strcat(in_buf,"\x0d\x0a");

	  len=7+strlen(fileName);

	  

	  if(write(sockid,in_buf,len)<0){

	    printf("write error\n");

	    exit(1);

	  }

	  if((n=read(sockid,recvline,MAXSIZE))<0){

	    printf("receive error\n");

	    exit(1);

	  }

	  

	  /* start to write the data to the server*/

	  if((fp=fopen(fileName, "r"))==NULL){

	    printf("File open error at the client side\n");

	    exit(1);

	  }

	  // obtain file size:

	  fseek (fp, 0 ,SEEK_END);

	  n = ftell (fp);

	  rewind (fp);

	  /*read in the data to buffer*/

	  if(fread (in_buf,1,n,fp)!=n){

	    printf("File read error!");

	    exit(1);

	  }

	  

	  if((j=write(newsockid,in_buf,n))<0){

	    printf("receive error\n");

	    exit(1);

	  }

	  close(newsockid);

	  fclose(fp);

	  

	  /* Read in server response and display it*/

	  if((n=read(sockid,recvline,MAXSIZE))<0){

	    printf("receive error\n");

	    exit(1);

	  }

	  printf("Server Response:",n);

	  for (i=0;i<n;i++){

	    printf("%c",recvline[i]);

	  } 	       
	}

	

	

	/*****************************************************************************

			command not supported

	*****************************************************************************/

	else{

	  printf("\nCommand not supported!\n");

	}

      }

      else{

	printf("\nCommand not supported!\n");

      }

    }

    

  }

  

  return 0;
}




// Code to communication with server on simple commands

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


// get the port number from PASV

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

	
