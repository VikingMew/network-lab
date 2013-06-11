#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include "csapp.h"
#include <string.h>
#include "rtlib.h"

#include "ircservice.h"
// #include "util.h"
// #include "network.h"
// #define MAX_MSG_LEN 512
#define LENPOOL 1
#define DAEMONFD fdl->next->fd
// static char  server_ip[MAX_MSG_LEN] = "175.186.60.211";
extern rt_config_entry_t *curr_node_config_entry;
extern rt_config_file_t   curr_node_config_file;
static char  daemon_ip[MAX_MSG_LEN] = "127.0.0.1";
int server_port;
int daemon_port;


struct fdlist *fdl;

int thread(struct fdlist *cur);

int init(int sockfd,int daemonfd)
{
    struct fdlist *cur;
    fdl = (struct fdlist *)malloc(sizeof(struct fdlist));
    fdl->fd = sockfd;
    cur = (struct fdlist *)malloc(sizeof(struct fdlist));
    fdl->next = cur;
    cur->fd = daemonfd;
    cur->next = 0;
    return 0;
}

int add(int sockfd,char* ipaddress)
{
    struct fdlist   *node;
    node = fdl;
    for(;node ->next != 0;node = node->next)
    {
        // assert(node->fd != sockfd);
    }
    struct fdlist *cur;
    cur = (struct fdlist *)malloc(sizeof(struct fdlist));
    bzero(cur,sizeof(struct fdlist));
    node->next = cur;
    cur->next = 0;
    cur->fd = sockfd;
    strcpy(cur->ipaddress,ipaddress);
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

struct fdlist* findbynickname(char nickname[MAX_MSG_LEN + 1])
{
    struct fdlist *node;
    node = fdl;
    for(;node != 0 && node ->next != 0;node = node -> next)
    {
        // assert(node -> next != 0);
        if(!strcmp(node->next->nickname,nickname)) 
        {
            return node->next;
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

int connect_daemon(int dport) {
    int daemonfd;
    struct sockaddr_in servaddr;
    daemonfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(dport);
    inet_aton(daemon_ip, &servaddr.sin_addr);
    printf( "SERVER IP %d\n", servaddr.sin_addr.s_addr );
    Connect(daemonfd, (struct sockaddr* )&servaddr, sizeof(servaddr));
    // should set sock to be non-blocking
    if (fcntl(daemonfd, F_SETFL, O_NONBLOCK) < 0){
        perror("fcntl (set non-blocking)");
        exit(1);
    }
    return daemonfd;
}

int connect_node(int nodeid) {

    int i;
    rt_config_entry_t *tmpentry;
    for( i = 0; i < curr_node_config_file.size; ++i )
        if( curr_node_config_file.entries[i].nodeID ==nodeid)
             tmpentry = &curr_node_config_file.entries[i];
    int nodefd;
    struct sockaddr_in servaddr;
    nodefd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(tmpentry->irc_port);
    inet_aton(tmpentry->ipaddr, &servaddr.sin_addr);
    printf( "connect to node IP %d\n", servaddr.sin_addr.s_addr );
    Connect(nodefd, (struct sockaddr* )&servaddr, sizeof(servaddr));
    // should set sock to be non-blocking
    if (fcntl(nodefd, F_SETFL, O_NONBLOCK) < 0){
        perror("fcntl (set non-blocking)");
        exit(1);
    }
    return nodefd;
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
    int daemonfd;
    int length;
    struct fdlist *cur;
    server_port = curr_node_config_entry->irc_port ;
    daemon_port = curr_node_config_entry->local_port;
    sockfd = Open_listenfd(server_port);
    daemonfd = connect_daemon(daemon_port);
    printf("daemon %d %d\n",daemon_port,daemonfd);
    printf("server_port %d %d\n",server_port,sockfd);
    // int opts = 1;
    // setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opts,sizeof(opts));
    init(sockfd,daemonfd);

    FD_ZERO(&fds);
    while(1)
    {
        FD_ZERO(&fds);
        // FD_SET(sockfd, &fds);
        max_fd = addtoset(&fds) + 1;
        // printf("max:%d\n",max_fd);
        ready = Select(max_fd, &fds, 0, 0, 0);
        if(ready == 0){
            continue;
        }
        int dealed = 0;
        for(cur = fdl->next;cur != 0 && cur -> next != 0 && !dealed;cur = cur -> next)
        {
            int fd = cur->next->fd;
            if(FD_ISSET(fd,&fds)) {
                // printf("select!:%d\n",cur->next->fd);
                if(thread(cur->next) == -1) {
                    // printf("delete!%d\n",fd);
                    Close(fd);
                    delete(fd);
                }
                dealed = 1;
            }
        }
        if(FD_ISSET(sockfd, &fds) && !dealed){
            // printf("detect!\n");
            csock = Accept(sockfd, (struct sockaddr*) &caddr, &size);
            if(getpeername(csock, (struct sockaddr*) &caddr, &size) == 0)
            {
                // printf("socket:%d\n",csock);
                // printf("ip:%s\n",inet_ntoa(caddr.sin_addr));
                char ip_address[MAX_MSG_LEN+1];
                bzero(&ip_address, sizeof(ip_address));
                sprintf(ip_address,"%s",inet_ntoa(caddr.sin_addr));
                add(csock,ip_address);
            }
            dealed = 1;
        }
        if(FD_ISSET(daemonfd, &fds) && !dealed){
            printf("daemon!\n");
            // length = rio_readlineb(&rp,recvbuf,MAX_MSG_LEN);
            // if(length<=0) app_error("d end!!!");
        }


    }
    return 0;
}

int thread(struct fdlist *cur)
{
    char recvbuf[MAX_MSG_LEN + 1];
    char sendbuf[MAX_MSG_LEN + 1];
    rio_t rp;
    int fd = cur->fd;
    int ret = 0;
    rio_readinitb(&rp,fd);
    int length = 1;
    bzero(&recvbuf, sizeof(recvbuf));
    bzero(&sendbuf, sizeof(sendbuf));
    length = rio_readlineb(&rp,recvbuf,MAX_MSG_LEN);
    if(length<=0) printf("end!!!");
    int i  = irc(recvbuf,sendbuf,cur);
    if(i == RPL_CLOSING) {
        ret = -1;
    }
    Rio_writen(fd,sendbuf,strlen(sendbuf));
    return ret;
}

int boardcast(char message[MAX_MSG_LEN+1])
{
    struct fdlist *cur;
    for(cur = fdl->next;cur != 0 && cur -> next != 0;cur = cur -> next)
    {
        Rio_writen(cur->next->fd,message,strlen(message));
    }
}

int boardcastbut(char message[MAX_MSG_LEN+1],int fd) // wont boardcast to fd
{  
    struct fdlist *cur;
    for(cur = fdl->next;cur != 0 && cur -> next != 0;cur = cur -> next)
    {
        if (fd != cur->next->fd) {
            Rio_writen(cur->next->fd,message,strlen(message));
        }
    }
    return 0;
}

int boardcastchannel(char message[MAX_MSG_LEN+1],char channel[MAX_MSG_LEN + 1])
{
    struct fdlist *cur;
    int issent = 0;
    for(cur = fdl->next;cur != 0 && cur -> next != 0;cur = cur -> next)
    {
        if (!strcmp(channel,cur->next->channel)) {
            rio_writen(cur->next->fd,message,strlen(message));
            issent = 1;
        }
    }
    return issent;
}
int sendmessage(char message[MAX_MSG_LEN+1],int fd)
{
    rio_writen(fd,message,strlen(message));
    return 0;
}
int sendtodaemon(char message[MAX_MSG_LEN+1],char recv[MAX_MSG_LEN+1])
{
    rio_writen(DAEMONFD,message,strlen(message));
    char recvbuf[MAX_MSG_LEN + 1];
    rio_t rp;
    int ret = 0;
    rio_readinitb(&rp,DAEMONFD);
    int length = 1;
    bzero(&recvbuf, sizeof(recvbuf));
    length = rio_readlineb(&rp,recvbuf,MAX_MSG_LEN);
    return length;
}

int existschannel(char channel[MAX_MSG_LEN + 1])
{
    struct fdlist *cur;
    int exist = 0;
    for(cur = fdl->next;cur != 0 && cur -> next != 0;cur = cur -> next)
    {
        if (!strcmp(channel,cur->next->channel)) {
            return 1;
        }
    }
    return 0;
}

int sendtonode(int nodeid,char message[MAX_MSG_LEN+1])
{
    int nodefd = connect_node(nodeid);
    Rio_writen(nodefd,message,strlen(message));
    Close(nodefd);
}