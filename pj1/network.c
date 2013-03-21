#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include "csapp.h"

// #include <stdlib.h>
#include <string.h>
// #include <stdio.h>
// #include <errno.h>
// #include <sys/times.h>
// #include <unistd.h>

#define MAX_MSG_LEN 512
#define LENPOOL 1

// static char  server_ip[MAX_MSG_LEN] = "175.186.60.211";
int server_port = 6667;

int init_listen()
{
    int sockfd;
    int csock;

    struct sockaddr_in caddr;
    int size;

    fd_set fds;
    int ready;
    int max_fd;

    sockfd = Open_listenfd(server_port);

    FD_ZERO(&fds);
    // FD_SET(sockfd, &read_fds);
    // max_fd = sockfd + 1;

    while(1)
    {
        FD_SET(sockfd, &fds);
        max_fd = sockfd + 1;
        ready = Select(max_fd, &fds, 0, 0, 0);
        if(ready == 0)
            continue;
        if(FD_ISSET(sockfd, &fds)){
            csock = Accept(sockfd, (struct sockaddr*) &caddr, &size);
            if(getpeername(csock, (struct sockaddr*) &caddr, &size) == 0)
            {
                printf("socket:%d\n",csock);
                printf("ip:%s\n",inet_ntoa(caddr.sin_addr));
                // thread(csock);
            }
        }
    }
    return 0;
}
int thread(int fd)
{
    char recvbuf[MAX_MSG_LEN + 1];
    rio_t rp;
    rio_readinitb(&rp,fd);
    int length = 1;
    bzero(&recvbuf, sizeof(recvbuf));
    length = Rio_readlineb(&rp,recvbuf,MAX_MSG_LEN);

    printf("%s",recvbuf);

    Rio_writen(fd,recvbuf,strlen(recvbuf));//echo it
    Close(fd);
    return 0;
}