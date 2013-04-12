int init_listen();
int boardcast(char message[MAX_MSG_LEN+1]);
int boardcastbut(char message[MAX_MSG_LEN+1],int fd);

struct fdlist findbynickname(char nickname[MAX_MSG_LEN + 1]);
// int writetoallfdbut(char message[MAX_MSG_LEN+1],int fd);
// int writetofd();
// struct fdlist *fdl;