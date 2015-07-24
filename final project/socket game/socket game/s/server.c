/*Junjun Huang
 cs433-final project
 server part final version 10/9
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <arpa/inet.h>

#define PORT 8888
#define MAX_CLIENT_NUMBER 5
#define LENGTH 1024

void game(int fd) {
    char buf[LENGTH];
    int len;
    char name[LENGTH], choice[LENGTH];
    int server_choice;
    int client_choice;
    srand(time(NULL));
    server_choice = rand() % 3 + 1;
    bzero(buf, LENGTH);
    // get the name of the client
    len = recv(fd, buf, LENGTH, 0);
    if (len < 0) {
        fprintf(stderr, "receive error 001\n");
        exit(1);
    }
    if (strncmp(buf, "<name>", 6) != 0) {
        fprintf(stderr, "choice error 002\n");
        return;
    }
    // send welcome message
    bzero(name, LENGTH);
    strcpy(name, buf + 6);
    strcpy(buf, "Hi ");
    strcat(buf, name);
    strcat(buf, "! Welcome to the game! ");
    len = send(fd, buf, strlen(buf), 0);
    if (len < 0) {
        fprintf(stderr, "send error 003\n");
        exit(errno);
    }
    bzero(buf, LENGTH);
    // get choice from client
    len = recv(fd, buf, LENGTH, 0);
    if (len < 0) {
        fprintf(stderr, "receive error 004\n");
        exit(1);
    }
    if (strncmp(buf, "<choice>", 8) != 0) {
        fprintf(stderr, "choice error 005\n");
        return;
    }
    bzero(choice, LENGTH);
    strcpy(choice, buf + 8);
    if (strncmp(choice, "Rock", 4) == 0) {
        client_choice = 1;
    } else if (strncmp(choice, "Paper", 5) == 0) {
        client_choice = 2;
    } else {
        client_choice = 3;
    }
    bzero(buf, LENGTH);
    if (server_choice == 1) {
        strcpy(buf, "Server's choice is 'Rock'. ");
    } else if (server_choice == 2) {
        strcpy(buf, "Server's choice is 'Paper'. ");
    } else {
        strcpy(buf, "Server's choice is 'Scissors'. ");
    }
    if (server_choice == client_choice) {
        strcat(buf, "The result is draw.");
    } else if (server_choice == 1 && client_choice == 2) {
        strcat(buf, "You win.");
    } else if (server_choice == 2 && client_choice == 3) {
        strcat(buf, "You win.");
    } else if (server_choice == 3 && client_choice == 1) {
        strcat(buf, "You win.");
    } else {
        strcat(buf, "You lose.");
    }
    // send result to client
    len = send(fd, buf, strlen(buf), 0);
    if (len < 0) {
        fprintf(stderr, "send error 006\n");
        exit(errno);
    }
}

int main(int argc, char** argv) {
    int sockfd, newfd;
    struct sockaddr_in s_addr, c_addr;
    socklen_t len;
    pid_t pid;
    
    // get socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "socket error 007\n");
        exit(errno);
    }
    bzero(&s_addr, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);
    s_addr.sin_addr.s_addr = INADDR_ANY;
    
    // bind port
    if ((bind(sockfd, (struct sockaddr*)&s_addr, sizeof(struct sockaddr))) == -1) {
        fprintf(stderr, "bind error 008\n");
        exit(errno);
    }
    
    // listen to clients
    if (listen(sockfd, MAX_CLIENT_NUMBER) == -1) {
        fprintf(stderr, "listen error 009\n");
        exit(errno);
    }
    
    while (1) {
        len = sizeof(struct sockaddr);
        newfd = accept(sockfd, (struct sockaddr*)&c_addr, &len);
        if (newfd == -1) {
            fprintf(stderr, "accept error 010\n");
            exit(errno);
        }
        pid = fork();
        // create subprocess to play game with client
        if (pid < 0) {
            fprintf(stderr, "fork error 011\n");
            exit(errno);
        }
        if (pid == 0) {
            game(newfd);
            close(newfd);
            exit(0);
        } else {
            close(newfd);
        }
    }
    
    return 0;
}
