/*Junjun Huang
 cs433-final project
 client part final version 10/9
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <arpa/inet.h>

#define IP "127.0.0.1"
#define PORT 8888
#define LENGTH 1024

void game(int fd) {
    char buf[LENGTH];
    int len;
    char name[LENGTH];
    int client_choice;
    bzero(buf, LENGTH);
    // read name of the client
    printf("Enter your name: ");
    strcpy(buf, "<name>");
    scanf("%s", name);
    strcat(buf, name);
    // send name of the client
    len = send(fd, buf, strlen(buf), 0);
    if (len < 0) {
        fprintf(stderr, "send error 001\n");
        exit(errno);
    }
    bzero(buf, LENGTH);
    // receive welcome message
    len = recv(fd, buf, LENGTH, 0);
    if (len < 0) {
        fprintf(stderr, "receive error 002\n");
        exit(errno);
    }
    printf("%s\n", buf);
    bzero(buf, LENGTH);
    // choose an option
    strcpy(buf, "<choice>");
    client_choice = 0;
    while (client_choice < 1 || client_choice > 3) {
        printf("1.Rock\n2.Paper\n3.Scissors\nEnter your choice:");
        scanf("%d", &client_choice);
        if (client_choice == 1) {
            strcat(buf, "Rock");
            break;
        } else if (client_choice == 2) {
            strcat(buf, "Paper");
            break;
        } else if (client_choice == 3) {
            strcat(buf, "Scissors");
            break;
        }
        fprintf(stderr, "Invalid choice, try again.\n");
    }
    // send choice to server
    len = send(fd, buf, strlen(buf), 0);
    if (len < 0) {
        fprintf(stderr, "send error 003\n");
        exit(errno);
    }
    bzero(buf, LENGTH);
    len = recv(fd, buf, LENGTH, 0);
    if (len < 0) {
        fprintf(stderr, "receive error 004\n");
        exit(errno);
    }
    printf("%s\n", buf);
}

int main(int argc, char** argv) {
    int sockfd;
    struct sockaddr_in s_addr;
    
    // get socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "socket error 005\n");
        exit(errno);
    }
    
    bzero(&s_addr, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);
    // set ip and port
    if (inet_aton(IP, (struct in_addr*)&s_addr.sin_addr.s_addr) == 0) {
        fprintf(stderr, "inet_aton error 006\n");
        exit(errno);
    }
    // connect to server
    if (connect(sockfd, (struct sockaddr*)&s_addr, sizeof(struct sockaddr)) == -1) {
        fprintf(stderr, "connect error 007\n");
        exit(errno);
    }
    
    game(sockfd);
    close(sockfd);
    
    return 0;
}
