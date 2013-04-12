#include "network.h"
#define MAX_MSG_TOKENS 10
#define MAX_MSG_LEN 512

//code from USER
#define ERR_NEEDMOREPARAMS 461
#define ERR_ALREADYREGISTRED 462
#define RPL_MOTDSTART 375
#define RPL_MOTD 372
#define RPL_ENDOFMOTD 376


//code from NICK
#define ERR_NONICKNAMEGIVEN 431
#define ERR_ERRONEUSNICKNAME 432
#define ERR_NICKNAMEINUSE 433

//code from QUIT
#define RPL_CLOSING 362
#define ERR_UNKNOWNCOMMAND 421

//code from JOIN
//All ignored by 15-441

// int nick(char* nickname);
// int user();
// int quit();
// int join();
// int part();
// int list();
// int privmsg();
// int who();
// int parse_msg(char token[MAX_MSG_TOKENS][MAX_MSG_LEN+1],int length

int irc(char recvbuf[MAX_MSG_LEN+1], char sendbuf[MAX_MSG_LEN+1],struct fdlist* cur);