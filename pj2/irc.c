#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include "csapp.h"
#include <string.h>
#include "rtlib.h"

#include "irc.h"
#include "routing.h"

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

/* Declare Helper Functions */
void trim(char* buf, char* msg);
int tokenize(char const *in_buf, char tokens[MAX_MSG_TOKENS][MAX_MSG_LEN+1]);
void read_msg(int fd, char *recvbuf);
void parse_msg(char tokens[MAX_MSG_TOKENS][MAX_MSG_LEN+1], int length, char *sendbuf, int fd);

/* Implements */

void do_irc(int fd) {

    char recvbuf[MAX_MSG_LEN + 1];
    char recvbuf2[MAX_MSG_LEN+1];
    char sendbuf[MAX_MSG_LEN + 1];

    char tokens[MAX_MSG_TOKENS][MAX_MSG_LEN+1];
    int length;

    bzero(&recvbuf, sizeof(recvbuf));
    bzero(&sendbuf, sizeof(sendbuf));

   	read_msg(fd, recvbuf);

    bzero(&recvbuf2, sizeof(recvbuf2));

    trim(recvbuf, recvbuf2);

   	length = tokenize(recvbuf2, tokens);

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
    	remove_user(fd, tokens[1], sendbuf);
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

    node_t *node;

    printf("Adding New User %s\n", nick);

    for (node = LocalIRCList.UserList.header; node != NULL; node = node->next) {
        printf("Existing User: %s", node->data);
        if (!strcasecmp(node->data, nick)) {
            printf("...Already Added!\n");
            goto Existing_User;
        }
        printf("\n");
    }
    node = malloc(sizeof(node_t));
    node->data = strdup(nick);
    node->next = LocalIRCList.UserList.header;
    LocalIRCList.UserList.header = node;
    LocalIRCList.UserList.Count++;
    printf("Added User: %s\n", node->data);

    // BroadcastSelf();

Existing_User:
	sprintf(sendbuf, "OK\r\n");
	Rio_writen(fd, sendbuf, strlen(sendbuf));
	return;

}
void add_channel(int fd, const char *channel, char *sendbuf) {

    node_t *node;

    printf("Adding New Channel %s\n", channel);

    for (node = LocalIRCList.ChannelList.header; node != NULL; node = node->next)
    {
        printf("Existing Channel: %s", node->data);
        if (!strcasecmp(node->data, channel)) {
            printf("...Already Added!\n");
            goto Existing_Channel;
        }
        printf("\n");
    }
    node = malloc(sizeof(node_t));
    node->data = strdup(channel);
    node->next = LocalIRCList.ChannelList.header;
    LocalIRCList.ChannelList.header = node;
    LocalIRCList.ChannelList.Count++;
    printf("Added Channel: %s\n", node->data);
    //BroadcastSelf();

Existing_Channel:
	sprintf(sendbuf, "OK\r\n");
	Rio_writen(fd, sendbuf, strlen(sendbuf));
	return;

}
void remove_user(int fd, const char *nick, char *sendbuf) {

    node_t *prev, *node;

    printf("Removing User %s\n", nick);

    for (prev = NULL, node = LocalIRCList.UserList.header; node != NULL; prev = node, node = node->next) {
        printf("Existing User: %s", node->data);
        if (strcasecmp(node->data, nick) == 0) {
            printf("...Found!\n");
            break;
        }
        printf("\n");
    }
    if (node != NULL) {
        if (prev == NULL)
            LocalIRCList.UserList.header = node->next;
        else
            prev->next = node->next;
        LocalIRCList.UserList.Count--;

        printf("Removed User: %s\n", node->data);
        free(node->data);
        free(node);       
        // BroadcastSelf();
        sprintf(sendbuf, "OK\r\n");
        Rio_writen(fd, sendbuf, strlen(sendbuf));
        return;
    }
    printf("Cannot Find User\n");
    sprintf(sendbuf, "OK\r\n");
    Rio_writen(fd, sendbuf, strlen(sendbuf));
	return;

}
void remove_channel(int fd, const char *channel, char *sendbuf) {

    node_t *prev, *node;

    printf("Removing Channel %s\n", channel);

    for (prev = NULL, node = LocalIRCList.ChannelList.header; node != NULL; prev = node, node = node->next) {
        printf("Existing Channel: %s", node->data);
        if (strcasecmp(node->data, channel) == 0) {
            printf("...Found!\n");
            break;
        }
        printf("\n");
    }
    if (node != NULL) {
        if (prev == NULL)
            LocalIRCList.ChannelList.header = node->next;
        else
            prev->next = node->next;
        LocalIRCList.ChannelList.Count--;

        printf("Removed Channel: %s\n", node->data);
        free(node->data);
        free(node);       
        // BroadcastSelf();
        sprintf(sendbuf, "OK\r\n");
        Rio_writen(fd, sendbuf, strlen(sendbuf));
        return;
    }
    printf("Cannot Find Channel\n");
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


void trim(char *buf, char *msg)
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
        return;
    }

    /* found a complete message */
    memcpy(msg, buf, len);
    msg[end-buf] = '\0';

    return; 
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