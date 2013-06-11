#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include "csapp.h"
#include <string.h>
#include "rtlib.h"

#include "irc.h"

/* Global variables */

extern u_long curr_nodeID;
extern rt_config_file_t   curr_node_config_file;  /* The config_file  for this node */
extern rt_config_entry_t *curr_node_config_entry; /* The config_entry for this node */

/* ===== OSPF OPTIONS ===== */
extern unsigned long curr_node_advertisement_cycle_time; /* -a advertisement cycle */
extern unsigned long curr_node_neighbor_timeout;         /* -n timeout for dead neighbors */
extern unsigned long curr_node_retransmission_timeout;   /* -r timeout for retransmission */
extern unsigned long curr_node_lsa_timeout;              /* -t timeout to expire an LSA */

/* Declare Processing Functions */
void add_user(int fd, const char *nick, char *sendbuf);
void add_channel(int fd, const char *channel, char *sendbuf);
void remove_user(int fd, const char *nick, char *sendbuf);
void remove_channel(int fd, const char *channel, char *sendbuf);
void next_hop(int fd, const char *nick, char *sendbuf);
void next_hops(int fd, int source_id, const char *channel, char *sendbuf);
void user_table(int fd, char *sendbuf);
void channel_table(int fd, char *sendbuf);

/* Implements */

void do_irc(int fd) {

    char recvbuf[MAX_MSG_LEN + 1];
    char sendbuf[MAX_MSG_LEN + 1];
    char tokens[MAX_MSG_TOKENS][MAX_MSG_LEN+1];
    int length;

    bzero(&recvbuf, sizeof(recvbuf));
    bzero(&sendbuf, sizeof(sendbuf));

   	read_msg(fd, recvbuf, sendbuf);

   	length = tokenize(recvbuf, tokens);

   	parse_msg(tokens, length, sendbuf, fd);



}

int tokenize( char const *in_buf, char tokens[MAX_MSG_TOKENS][MAX_MSG_LEN+1])
{
    int i = 0;
    const char *current = in_buf;
    int  done = 0;

    /* Possible Bug: handling of too many args */
    while (!done && (i<MAX_MSG_TOKENS)) {
        char *next = strchr(current, ' ');
        if (next) {
            memcpy(tokens[i], current, next-current);
            tokens[i][next-current] = '\0';
            current = next + 1;   /* move over the space */
            ++i;

            /* trailing token */
            if (*current == ':') {
                ++current;
            strcpy(tokens[i], current);
            ++i;
            done = 1;
            }
        } else {
            strcpy(tokens[i], current);
            ++i;
            done = 1;
        }
    }

    return i;
}

void parse_msg(char tokens[MAX_MSG_TOKENS][MAX_MSG_LEN+1],
              int length, char *sendbuf, int fd)
{
    // app_error("Not Implemented");
    // int i = ERR_UNKNOWNCOMMAND; 
    if(!strcmp(tokens[0],"ADDUSER"))
    {
        add_user(fd, tokens[1], sendbuf);
    } else
    if(!strcmp(tokens[0],"ADDCHAN"))
    {
    	add_channel(fd, tokens[1], sendbuf);
    } else
    if(!strcmp(tokens[0],"REMOVEUSER"))
    {
    	remove_channel(fd, tokens[1], sendbuf);
    } else
    if(!strcmp(tokens[0],"REMOVECHAN"))
    {
    	remove_channel(fd, tokens[1], sendbuf);
    } else
    if(!strcmp(tokens[0],"NEXTHOP"))
    {
    	next_hop(fd, tokens[1], sendbuf);
    } else
    if(!strcmp(tokens[0],"NEXTHOPS"))
    {
    	next_hops(fd, atol(tokens[1]), tokens[2], sendbuf);
    } else
    if(!strcmp(tokens[0],"USERTABLE"))
    {
    	user_table(fd, sendbuf);
    } else
    if(!strcmp(tokens[0],"CHANTABLE"))
    {
    	channel_table(fd, sendbuf);
    } else {

    }
    return;
}

void add_user(int fd, const char *nick, char *sendbuf) {

	sprintf(sendbuf, "OK\r\n");
	Rio_writen(fd, sendbuf, strlen(sendbuf));
	return;

}
void add_channel(int fd, const char *channel, char *sendbuf) {

	sprintf(sendbuf, "OK\r\n");
	Rio_writen(fd, sendbuf, strlen(sendbuf));
	return;

}
void remove_user(int fd, const char *nick, char *sendbuf) {

	sprintf(sendbuf, "OK\r\n");
	Rio_writen(fd, sendbuf, strlen(sendbuf));
	return;

}
void remove_channel(int fd, const char *channel, char *sendbuf) {

	sprintf(sendbuf, "OK\r\n");
	Rio_writen(fd, sendbuf, strlen(sendbuf));
	return;
	
}
void next_hop(int fd, const char *nick, char *sendbuf) {

}
void next_hops(int fd, int source_id, const char *channel, char *sendbuf) {

}
void user_table(int fd, char *sendbuf) {

}
void channel_table(int fd, char *sendbuf) {

}


size_t get_msg(char *buf, char *msg)
{
    char *end;
    int  len;

    /* Find end of message */
    end = strstr(buf, "\r\n");

    if( end )
    {
        len = end - buf + 2;
    }
    else
    {
        /* Could not find \r\n, try searching only for \n */
        end = strstr(buf, "\n");
    if( end )
        len = end - buf + 1;
    else
        return -1;
    }

    /* found a complete message */
    memcpy(msg, buf, len);
    msg[end-buf] = '\0';

    return len; 
}

void read_msg(int fd, char *recvbuf)
{

    rio_t rp;
    int ret = 0;
    rio_readinitb(&rp,fd);
    int length = 1;
    length = rio_readlineb(&rp,recvbuf,MAX_MSG_LEN);
    if(length<=0) printf("end!!!");
    return;
}