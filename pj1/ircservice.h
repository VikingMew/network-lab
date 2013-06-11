#include "network.h"
#define MAX_MSG_TOKENS 10
#define MAX_MSG_LEN 512

//code from USER
#define ERR_NEEDMOREPARAMS      461
#define ERR_ALREADYREGISTRED    462
#define RPL_MOTDSTART           375
#define RPL_MOTD                372
#define RPL_ENDOFMOTD           376
#

//code from NICK
#define ERR_NONICKNAMEGIVEN     431
#define ERR_ERRONEUSNICKNAME    432
#define ERR_NICKNAMEINUSE       433

//code from QUIT
#define RPL_CLOSING             362
#define ERR_UNKNOWNCOMMAND      421

//code from JOIN
#define RPL_NAMEREPLY           353
#define RPL_ENDOFNAMES          366
#define ERR_TOOMANYCHANNELS     405

//code from WHO
#define RPL_WHOREPLY            352
#define RPL_ENDOFWHO            315
//Error all ignored by 15-441

//code from LIST
#define ERR_NOSUCHSERVER        402
#define RPL_LISTSTART           321
#define RPL_LIST                322
#define RPL_LISTEND             323

//code from PRIVMSG
#define ERR_NOSUCHNICK          401
#define ERR_NOTEXTTOSEND        412

//code from PART
#define ERR_NOTONCHANNEL        442
#define ERR_NOSUCHCHANNEL       403

int irc(char recvbuf[MAX_MSG_LEN+1], char sendbuf[MAX_MSG_LEN+1],struct fdlist* cur);