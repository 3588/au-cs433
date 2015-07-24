int socket(int protofamily,      // PF_INET or PF_UNIX
		   int type,                //  SOCK_STREAM or SOCK_DGRAM
		   int protocol         //  IPPROTO_TCP or IPPROTO_UDP or 0 (use default)
           );
int connect(int socket,    // file descriptor of a (local) socket
			struct sockaddr * saddr, // pointer to a structure containing the address of a remote socket
			int saddrlen  // length (in bytes) of the sockaddr struct
            );
struct sockaddr {
    u_char   sa_len;             // length of the struct
    u_char   sa_family;         // address family
    char       sa_data[14];    // actual address; format depends on address family
    };
struct sockaddr_in {
    u_char   sin_len;               //  length of the struct
    u_char   sin_family;          //  address family (AF_INET)
    u_short   sin_port;            //  port number
    struct in_addr sin_addr;    //  32-bit binary IP address
    char        sin_zero[8];       //  set to zero
    };
int bind(int sockfd,               //  file descriptor for a local socket
		 struct sockaddr * saddr,  //  socket address
		 int addrlen            //  length of sockaddr struct
         );
int listen(int sockfd,     //  file descriptor for a local socket
		   int backlog   //  length of queue for waiting clients
          );

int accept(int sockfd,   //  file descriptor for a listening socket
		   struct sockaddr * saddr,//address of struct to be filled in with client's socket address
		   int * addrlen //address of local int to be filled in with length of client's socket address
		   );
int read(int fd,         //  file descriptor to read
		 char * buffer,   //  address of a local variable to be filled with incoming data
		 int len);      //   number of bytes to read

int write(int fd,         //  file descriptor to write
		  char * buffer,      //  address of a local variable containing data to be written
		  int len);      //  number of bytes to write

int send(int fd, //file descriptor to write to
		 char * buffer,//message to write
		 int len,//size of the message
		 int flags//usually set to 0
		 );
int recv(int fd,//file descriptor to read from
		 char * buffer,//where the message will be stored
		 int len,//size of the buffer
		 int flags//usually set to 0.
		 );

int inet_aton(const char *cp, //cp contains the ip address such as 172.26.91.120
			struct in_addr *inp)//contains the binary  form (in network byte order) after conversion


ssize_t sendto(	int s, //sender's own socket
				const void *buf, //buffer that stores the message to send
				size_t len, //size of the buffer above
				int flags, //usually set to be 0
				const struct sockaddr *to,//the destination address to send the message to
				socklen_t tolen //size of the destination address
				);


 ssize_t recvfrom(int s, //receiver's own socket
				  void *buf, //buffer to receive the message
				  size_t len, //size of the buffer
				  int flags,//usually set to be 0
				  struct sockaddr *from, //the source address to receive from
				  socklen_t *fromlen//size of the receive from address
				  );
