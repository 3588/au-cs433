
#include <sys/types.h> 
#include <sys/socket.h> // together with types.h, provide functions for socket(),bind(),listen() and accept()
#include <netinet/in.h> // defines internet address struct
#include <arpa/inet.h> //inet_aton() and inet_ntoa();
#include <netdb.h>// gethostbyname() and gethostbyaddr()
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> // for perror() and I/O

#define PORTNUMBER       6000           /* default protocol port number */
#define MAXLINE          1000           /* length of line buffer        */

int main(int argc, char**argv){
	struct sockaddr_in sad;
	memset((char*)&sad.sin_zero, 0, sizeof(sad.sin_zero));//clear sockaddr structure
	sad.sin_family=AF_INET;//set family to internet
	sad.sin_port=htons((u_short)PORTNUMBER);//convert port number to network byte order
	char input[MAXLINE];//store the input from a user
	struct hostent *hptr;//a pointer to be returned by gethostbyaddr
	char choice;
	
	while(1){
		printf("Enter an IP address[i] or host name[h]: ");
		scanf("%c", &choice);
		if(choice=='i'){
			scanf("%s", input);
			sad.sin_addr.s_addr=inet_addr(input);
			// or inet_aton(IP, &(sad.sin_addr));
	
			printf("%s 's internal representation: %d\n", input, sad.sin_addr.s_addr);
		
			//print host name if any
			if((hptr=gethostbyaddr((char*)&sad.sin_addr, sizeof(struct sockaddr_in), AF_INET))!=0){//if there is name returned
				printf("Host name: %s\n", hptr->h_name);
			
			}
		}
		else{
			scanf("%s", input);
			if((hptr=gethostbyname(input))==0){
				perror("gethostbyname");
			}
			else{
				printf("Host name: %s\n", hptr->h_name);
				printf("IP address: %s\n", inet_ntoa(*((struct in_addr *)hptr->h_addr)));
			}
			
		}
		scanf("%c", &choice);//get rid of the \n after the input. 
		
	}
	scanf("%c", & choice);
	return 0;
}


