#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include "csapp.h"
#include <string.h>

#include "ircservice.h"
#include "util.h"
// #define MAX_MSG_LEN 512
#define LENPOOL 1

// static char  server_ip[MAX_MSG_LEN] = "175.186.60.211";
int server_port = 6667;


struct fdlist
{
    int fd;
    char[] nickname;
    struct fdlist *next;
};

struct fdlist *fdl;

int thread(int fd);

int init(int sockfd)
{
    fdl = (struct fdlist *)malloc(sizeof(struct fdlist));
    fdl->fd = sockfd;
    fdl->next = 0;
    return 0;
}

int add(int sockfd)
{
    struct fdlist   *node;
    node = fdl;
    for(;node ->next != 0;node = node->next)
    {
        // assert(node->fd != sockfd);

    }
    struct fdlist *cur;
    cur = (struct fdlist *)malloc(sizeof(struct fdlist));
    node->next = cur;
    cur->next = 0;
    cur->fd = sockfd;
    return 0;
}

int delete(int sockfd)
{
    struct fdlist *node;
    node = fdl;
    for(;node ->next != 0;node = node -> next)
    {
        // assert(node -> next != 0);
        int fd = node -> next ->fd;
        if(fd == sockfd) 
        {
            struct fdlist *cur;
            cur = node->next;
            node->next = cur->next;
            free(cur);
            return 0;
        }
    }
    return -1;
}

struct fdlist findbynickname(char nickname[MAX_MSG_LEN + 1])
{
    struct fdlist *node;
    node = fdl;
    for(;node ->next != 0;node = node -> next)
    {
        // assert(node -> next != 0);
        if(str) 
        {
            struct fdlist *cur;
            cur = node->next;
            node->next = cur->next;
            free(cur);
            
        }
    }
    return 0;
}

int addtoset(fd_set *set)
{
    struct fdlist *node;
    int max = 0;
    for(node = fdl; node != 0; node = node -> next)
    {
        int fd = node->fd;
        if(fd > max) 
        {
            max = fd;
        }
        FD_SET(fd, set);
    }
    return max;
}

int init_server()
{
    int sockfd;
    int csock;

    struct sockaddr_in caddr;
    int size;

    fd_set fds;
    int ready;
    int max_fd;
    struct fdlist *cur;

    sockfd = Open_listenfd(server_port);
    int opts = 1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opts,sizeof(opts));
    init(sockfd);
    FD_ZERO(&fds);
    while(1)
    {
        // FD_SET(sockfd, &fds);
        max_fd = addtoset(&fds)+1;
        // printf("max:%d\n",max_fd);
        ready = Select(max_fd, &fds, 0, 0, 0);
        if(ready == 0){
            continue;
        }
        for(cur = fdl;cur != 0 && cur -> next != 0;cur = cur -> next)
        {
            int fd = cur->next->fd;
            if(FD_ISSET(fd,&fds)) {
                // printf("select!:%d\n",cur->fd);
                if(thread(cur) == -1) {
                    printf("delete!%d\n",fd);
                    Close(fd);
                    delete(fd);
                }
            }
        }
        if(FD_ISSET(sockfd, &fds)){
            printf("detect!\n");
            csock = Accept(sockfd, (struct sockaddr*) &caddr, &size);
            if(getpeername(csock, (struct sockaddr*) &caddr, &size) == 0)
            {
                printf("socket:%d\n",csock);
                printf("ip:%s\n",inet_ntoa(caddr.sin_addr));
                add(csock);
            }
        }

    }
    return 0;
}

int thread(struct fdlist* cur)
{
    char recvbuf[MAX_MSG_LEN + 1];
    char sendbuf[MAX_MSG_LEN + 1];
    char globbuf[MAX_MSG_LEN + 1];
    rio_t rp;
    rio_readinitb(&rp,fd);
    int length = 1;
    bzero(&recvbuf, sizeof(recvbuf));
    bzero(&sendbuf, sizeof(recvbuf));
    bzero(&globbuf, sizeof(globbuf));
    length = Rio_readlineb(&rp,recvbuf,MAX_MSG_LEN);
    printf("%s",recvbuf);
    ret = irc(recvbuf,sendbuf,cur);
    if(ret == 362) {
        return -1;
    }
    Rio_writen(fd,sendbuf,strlen(sendbuf));
    return 0;
}

int boardcast(char message[MAX_MSG_LEN+1]) {

}

int boardcastbut(char message[MAX_MSG_LEN+1],int fd) {

}
