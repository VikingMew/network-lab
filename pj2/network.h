#include "sircd.h"
struct fdlist
{
    int fd;
    char nickname[MAX_MSG_LEN + 1];
    char username[MAX_MSG_LEN + 1];
    char hostname[MAX_MSG_LEN + 1];
    char servername[MAX_MSG_LEN + 1];
    char realname[MAX_MSG_LEN + 1];
    char ipaddress[MAX_MSG_LEN + 1];
    char channel[MAX_MSG_LEN + 1];
    struct fdlist *next;
};

int init_listen();
int sendmessage(char message[MAX_MSG_LEN+1],int fd);
int boardcast(char message[MAX_MSG_LEN+1]);
int boardcastbut(char message[MAX_MSG_LEN+1],int fd);
int boardcastchannel(char message[MAX_MSG_LEN+1],char channel[MAX_MSG_LEN + 1]);

int existschannel(char channel[MAX_MSG_LEN + 1]);
int sendtodaemon(char message[MAX_MSG_LEN+1],char recv[MAX_MSG_LEN+1]);

struct fdlist* findbynickname(char nickname[MAX_MSG_LEN + 1]);
// int writetoallfdbut(char message[MAX_MSG_LEN+1],int fd);
// int writetofd();
// struct fdlist *fdl;