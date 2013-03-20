#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include "csapp.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
// #include <arpa/inet.h>
#include <sys/times.h>
// #include <sys/types.h>
// #include <sys/socket.h>
#include <unistd.h>
// #include "csapp.h"

#define MAX_MSG_LEN 512
#define LENPOOL 1

static char  server_ip[MAX_MSG_LEN] = "127.0.0.1";
int          server_port = 6667;

int init_listen()
{
    int sockfd;
    int csock;
    // sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    char recvbuf[MAX_MSG_LEN + 1];
    sockfd = Open_listenfd(server_port);
    struct sockaddr_in caddr;
    // struct sockaddr_in servaddr;

    // bzero(&servaddr, sizeof(servaddr));
    // servaddr.sin_family = AF_INET;
    // servaddr.sin_port = htons(server_port);
    // inet_aton(server_ip, &servaddr.sin_addr);

    // Bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    // Listen(sockfd, 1);
    // char* recbuf;
    int size;
    // printf("Server IP %d\n", servaddr.sin_addr.s_addr);
    while(1)
    {
        printf("accept?\n");
        csock = Accept(sockfd, (struct sockaddr*) &caddr, &size);
        printf("size:%d\n",caddr.sin_addr);
        rio_t rp;
        rio_readinitb(&rp,csock);
        while(1) {
        bzero(&recvbuf, sizeof(recvbuf));
        Rio_readlineb(&rp,recvbuf,MAX_MSG_LEN);
        printf("%s",recvbuf);
        Rio_writen(csock,recvbuf,strlen(recvbuf));
        }
    }
    return 0;
}