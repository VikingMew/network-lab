#define MAX_MSG_TOKENS 10
#define MAX_MSG_LEN 512



//code from NICK
#define ERR_NONICKNAMEGIVEN 431
#define ERR_ERRONEUSNICKNAME 432
#define ERR_NICKNAMEINUSE 433

//code from QUIT
#define RPL_CLOSING 362
// int nick(char* nickname);
// int user();
// int quit();
// int join();
// int part();
// int list();
// int privmsg();
// int who();
// int parse_msg(char token[MAX_MSG_TOKENS][MAX_MSG_LEN+1],int length);
int irc(char recvbuf[MAX_MSG_LEN+1], char sendbuf[MAX_MSG_LEN+1]);