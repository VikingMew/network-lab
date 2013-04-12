#include "csapp.h"
#include "ircservice.h"
#include "sircd.h"
#include "network.h"

extern struct fdlist *fdl;


/* int setnickname(char* nickname)
 *
 * set the nickname to the curclient
 * Command:   NICK
 * Parameters:     <nickname> [ <hopcount> ]
 * error ERR_NICKNAMEINUSE ERR_NONICKNAMEGIVEN
 * return errcode when ERR
 * return 0 while success
 */


int nick(char nickname[MAX_MSG_LEN+1],
         char sendbuf[MAX_MSG_LEN+1],
         struct fdlist *cur)
{
    if(strlen(nickname) == 0)
    {
        return ERR_NONICKNAMEGIVEN;
    }
    struct fdlist *node = findbynickname(nickname);
    if(node != NULL)
    {
        return ERR_NICKNAMEINUSE;
    }
    strcpy(cur->nickname,nickname);
    char buf[MAX_MSG_LEN + 1];
    sprintf(buf,":%s!%s NICK %s",nickname,SERVERNAME,nickname);
    boardcastbut(buf,cur->fd);
    return 0;
    // app_error("Not Implemented");
    // return -1;
}

/* int setuser()
 *
 * Command:  USER
 * Parameters:     <username> <hostname> <servername> <realname>
 * set Specify the username, hostname, and real name of a user.
 * error ERR_NEEDMOREPARAMS     ERR_ALREADYREGISTRED
 * return -1 while error
 * return 0 while success
 */
int user()
{
    app_error("Not Implemented");
    return -1;
}

/* int quit()
 * 
 * Command:     QUIT
 * Parameters:     [<Quit message>]
 * quit the server
 * boardcast to others while quitting
 * while this function is invoked,quit wont be connection close
 * return 0 
 */
int quit(char msg[MAX_MSG_LEN+1],
         char sendbuf[MAX_MSG_LEN+1],
         struct fdlist *cur)
{
    sendbuf[0] = 0;
    char buf[MAX_MSG_LEN+1];
    char nickname[MAX_MSG_LEN+1];
    char sendmsg[MAX_MSG_LEN+1];
    // if(strlen(msg) == 0)
    // {
    //     strcpy(sendmsg,cur->nickname);
    // }
    // else
    // {
    //     strcpy(sendmsg,msg);
    // }
    // strcpy(nickname,cur->nickname);
    // sprintf(buf,":%s!%s QUIT %s",nickname,"localhost",sendmsg);
    // boardcast(buf);
    return 0;
    // app_error("Not Implemented");
    // return -1;
}

/* int join()
 *
 * Command:    JOIN
 * Parameters:     <channel>{,<channel>} [<key>{,<key>}]
 * error 
 *   ERR_NEEDMOREPARAMS  ERR_BANNEDFROMCHAN
 *   ERR_INVITEONLYCHAN  ERR_BADCHANNELKEY
 *   ERR_CHANNELISFULL   ERR_BADCHANMASK
 *   ERR_NOSUCHCHANNEL   ERR_TOOMANYCHANNELS
 *   RPL_TOPIC           RPL_NAMREPLY
 *   RPL_ENDOFNAMES 
 * return ?? when error
 * return 0 on success
 */
int join()
{
    app_error("Not Implemented");
    return -1;
}
/* int part()
 *
 * Command:     PART
 * Parameters:     <channel>{,<channel>}
 * ERR_NEEDMOREPARAMS
 * ERR_NOSUCHCHANNEL
 * ERR_NOTONCHANNEL 
 * return ?? when error
 * return 0 on success
 */
int part()
{
    app_error("Not Implemented");
    return -1;
}
/* int list()
 *
 * Command:     LIST
 * Parameters:     [<channel>{,<channel>} [<server>]]
 * ERR_NOSUCHSERVER   
 * RPL_LISTSTART   RPL_LIST
 * RPL_LISTEND 
 * return ?? when error
 * return 0 on success
 */
int list()
{

}
/* int privmsg()
 *
 * Command:  PRIVMSG
 * Parameters:     <target>{,<target>} <text to be sent>
 * ERR_NORECIPIENT     ERR_NOTEXTTOSEND
 * ERR_CANNOTSENDTOCHAN    ERR_NOTOPLEVEL
 * ERR_WILDTOPLEVEL    ERR_TOOMANYTARGETS
 * ERR_NOSUCHNICK  
 * RPL_AWAY 
 * return ?? when error
 * return 0 on success
 */
int privmsg()
{

}
/* int who()
 *
 * Command:   WHO
 * Parameters:     [<name>{,<name>} [<o>]]
 *
 * ERR_NOSUCHSERVER    
 * RPL_WHOREPLY    RPL_ENDOFWHO
 * return ?? when error
 * return 0 on success
 */
int who()
{

}
/* int parse_msg()
 * parse the tokenized message to the functions
 * call the functions should be called.
 * returns the type of msg
 * NICK - Give the user a nickname or change the previous one. not duplicate
 * USER - Specify the username, hostname, and real name of a user.
 * QUIT - End the client session.announce the client's departure to other users
 * JOIN - Start listening to a specific channel.
 * PART - Depart a specific channel.
 * LIST - List all existing channels and the number of users.ignore parameters

 * PRIVMSG - Send messages to users. The target can be either a nickname
 *   or a channel. If the target is a channel, the message will be broadcast 
 *   to every user on the specified channel, except the message originator. 
 *   If the target is a nickname, the message will be sent only to that user.

 * WHO - Query information about clients or channels.
 *   querying channels on the local server.
 *   It should do an exact match on the channel
 *   name and return the users on that channel.

 * return -1 while cant find the msg type 
 * return err type while function returns error
 */
int parse_msg(char token[MAX_MSG_TOKENS][MAX_MSG_LEN+1],
              int length, char sendbuf[MAX_MSG_LEN+1],
              struct fdlist *cur)
{
    // app_error("Not Implemented");
    if(length == 1 && !strcmp(token[0],""))
    {
        char buf[MAX_MSG_LEN+1];
        sprintf(buf,":%s!%s QUIT :Connection closed",cur->nickname,"localhost");
        boardcastbut(buf,cur->fd);
        return RPL_CLOSING;
    }
    if(!strcmp(token[0],"QUIT"))
    {
        quit(token[1],sendbuf,cur);
        return RPL_CLOSING;
    }
    if(!strcmp(token[0],"NICK"))
    {
        int i = nick(token[1],sendbuf,cur);
        return i;
    }
    return -1;
}

/*
 * int tokenize( char const *in_buf, char tokens[MAX_MSG_TOKENS][MAX_MSG_LEN+1] )
 *
 * A strtok() variant.  If in_buf is a space-separated list of words,
 * then on return tokens[X] will contain the Xth word in in_buf.
 *
 * Note: You might want to look at the first word in tokens to
 * determine what action to take next.
 *
 * Returns the number of tokens parsed.
 */
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



/*
 * size_t get_msg( char *buf, char *msg )
 *
 * char *buf : the buffer containing the text to be parsed
 * char *msg : a user malloc'ed buffer to which get_msg will copy the message
 *
 * Copies all the characters from buf[0] up to and including the first instance
 * of the IRC endline characters "\r\n" into msg.  msg should be at least as
 * large as buf to prevent overflow.
 *
 * Returns the size of the message copied to msg.
 */
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


int irc(char recvbuf[MAX_MSG_LEN+1], 
        char sendbuf[MAX_MSG_LEN+1],struct fdlist* cur)
{
    // get_msg(sendbuf,recvbuf);//echo it
    int tok_len;
    char tok[MAX_MSG_TOKENS][MAX_MSG_LEN+1];
    bzero(&tok, sizeof(tok));
    tok_len = tokenize(recvbuf,tok);
    int i = parse_msg(tok,tok_len,char sendbuf[MAX_MSG_LEN+1],cur)
    return i;
    
}

