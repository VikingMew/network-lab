#include "csapp.h"
#include "ircservice.h"
#include "sircd.h"
// #include "network.h"

extern struct fdlist *fdl;

int getnames(char channel[MAX_MSG_LEN+1],char buf[MAX_MSG_LEN+1])
{

}
int setmessage(int code,char command[MAX_MSG_LEN+1],
             char sendbuf[MAX_MSG_LEN+1],struct fdlist *cur)
{
    if(code == RPL_MOTDSTART)
    {
        sprintf(sendbuf,":%s 375 %s :- %s Message of the day -\r\n",
            SERVERNAME,cur->nickname,SERVERNAME);
    }
    if(code == RPL_MOTD)
    {
        sprintf(sendbuf,":%s 372 %s :- I have done my 15-441 yeah!\r\n",
            SERVERNAME,cur->nickname);
    }
    if(code == RPL_ENDOFMOTD)
    {
        sprintf(sendbuf,":%s 376 %s :End of /MOTD command\r\n",
            SERVERNAME,cur->nickname);
    }
    if(code == ERR_UNKNOWNCOMMAND)
    {
        sprintf(sendbuf,":%s 421 %s %s :Unknown command\r\n",
            SERVERNAME,cur->nickname,command);
    }
    if(code == ERR_NEEDMOREPARAMS)
    {
        sprintf(sendbuf,":%s 461 %s %s :Not enough parameters\r\n",
            SERVERNAME,cur->nickname,command);
    }
    if(code == RPL_ENDOFNAMES)   //'command' is channelname here!!!!
    {
        sprintf(sendbuf,":%s 366 %s %s :End of /NAMES list\r\n",
            SERVERNAME,cur->nickname,command);
    }
}

int motd(struct fdlist *cur) 
{
    char buf[MAX_MSG_LEN+1];
    bzero(&buf, sizeof(buf));
    setmessage(RPL_MOTDSTART,"",buf,cur);
    sendmessage(buf,cur->fd);
    bzero(&buf, sizeof(buf));
    setmessage(RPL_MOTD,"",buf,cur);
    sendmessage(buf,cur->fd);
    bzero(&buf, sizeof(buf));
    setmessage(RPL_ENDOFMOTD,"",buf,cur);
    sendmessage(buf,cur->fd);
    return 0;
}
/* int nick(char* nickname)
 *
 * set the nickname to the curclient
 * Command:   NICK
 * Parameters:     <nickname> [ <hopcount> ]
 * error ERR_NICKNAMEINUSE ERR_NONICKNAMEGIVEN
 * return errcode when ERR
 * return 0 while success
 */
int nick(char nickname[MAX_MSG_LEN+1],
         struct fdlist *cur)
{
    if(strlen(nickname) == 0)
    {
        return ERR_NONICKNAMEGIVEN;
    }
    struct fdlist *node = findbynickname(nickname);
    if(node != NULL)
    {
        return ERR_NICKNAMEINUSE;   //via 15-441
    }
    strcpy(cur->nickname,nickname);
    char buf[MAX_MSG_LEN + 1];
    sprintf(buf,":%s!%s NICK %s\n",nickname,SERVERNAME,nickname);
    boardcastbut(buf,cur->fd);
    if(strlen(cur->username))
    {
        motd(cur);
    }
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
int user(char username[MAX_MSG_LEN+1],
         char hostname[MAX_MSG_LEN+1], //ignored by 15-441
         char servername[MAX_MSG_LEN+1],
         char realname[MAX_MSG_LEN+1],
         struct fdlist *cur)
{
    if(!(strlen(username) && strlen(hostname) && 
         strlen(servername) && strlen(realname)))
    {
        return ERR_NEEDMOREPARAMS;
    }
    if(strlen(cur->username))
    {
        return ERR_ALREADYREGISTRED;

    }
    strcpy(cur->username,username);
    strcpy(cur->hostname,cur->ipaddress); //15-441
    strcpy(cur->servername,username);
    strcpy(cur->username,username);
    if(strlen(cur->nickname))
    {
        motd(cur);
    }
    return 0;
    // app_error("Not Implemented");
    // return -1;
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
         struct fdlist *cur)
{
    // sendbuf[0] = 0;
    char buf[MAX_MSG_LEN+1];
    char nickname[MAX_MSG_LEN+1];
    char sendmsg[MAX_MSG_LEN+1];
    //TODO: QUIT MSG
    // if(strlen(msg) == 0)
    // {
    //     strcpy(sendmsg,cur->nickname);
    // }
    // else
    // {
    //     strcpy(sendmsg,msg);
    // }
    // strcpy(nickname,cur->nickname);
    // sprintf(buf,":%s!%s QUIT %s\n",nickname,"localhost",sendmsg);
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
 *   ERR_NEEDMOREPARAMS 
 *   RPL_TOPIC           RPL_NAMREPLY
 *   RPL_ENDOFNAMES 
 * return ?? when error
 * return 0 on success
 */
int join(char channel[MAX_MSG_LEN+1],
         char key[MAX_MSG_LEN+1],struct fdlist *cur) //ignore by 15-441
{
    if(!strlen(channel))
    {
        return ERR_NEEDMOREPARAMS;
    }
    char *current = channel;
    printf("%s",channel);
    char channelname[MAX_MSG_LEN+1];
    bzero(&channelname,sizeof(channelname));
    while(strlen(channelname) == 0)
    {
        bzero(&channelname,sizeof(channelname));
        char *next = strchr(current, ' ');
        if (next)
        {
            memcpy(channelname, current, next-current); // get the parameter
            current = next + 1;
        }
        else 
        {
            strcpy(channelname, current);
            break;
        }
    }
    if(!strlen(channelname))
    {
        return ERR_NEEDMOREPARAMS;
    }
    if(channelname[0] != '&' && channelname[0] != '#')
    {
        return ERR_NEEDMOREPARAMS;
    }
    char oldchannel[MAX_MSG_LEN+1];
    if(strlen(cur->channel)) 
    {
        strcpy(oldchannel, cur->channel);
        //  TODO: PART from old
    }
    bzero(cur->channel, sizeof(cur->channel));
    strcpy(cur->channel, channelname);
    char buf[MAX_MSG_LEN+1];
    sprintf(buf, ":%s JOIN %s", cur->nickname, channelname);
    boardcastchannel(buf,channelname);
    bzero(&buf, sizeof(buf));
    sendmessage(buf,cur->fd);
    bzero(&buf, sizeof(buf));
    setmessage(RPL_ENDOFNAMES,channelname,buf,cur);
    sendmessage(buf,cur->fd);
    return 0;
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

    app_error("Not Implemented");
    return -1;
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
    app_error("Not Implemented");
    return -1;
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
    app_error("Not Implemented");
    return -1;
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
    int i = ERR_UNKNOWNCOMMAND; 
    if(length == 1 && !strcmp(token[0],""))
    {
        // char buf[MAX_MSG_LEN+1];
        // bzero(&buf, sizeof(buf));
        // sprintf(buf,":%s!%s QUIT :Connection closed\n",cur->nickname,"localhost");
        // boardcastbut(buf,cur->fd);
        i = RPL_CLOSING;
    } else
    if(!strcmp(token[0],"QUIT"))
    {
        quit(token[1],cur);
        i = RPL_CLOSING;
    } else
    if(!strcmp(token[0],"NICK"))
    {
        i = nick(token[1],cur);
    } else
    if(!strcmp(token[0],"USER"))
    {
        i = user(token[1],token[2],token[3],token[4],cur);
    }
    if(!strcmp(token[0],"JOIN"))
    {
        i = join(token[1],token[2],cur);
    }
    setmessage(i,token[0],sendbuf,cur);
    return i;
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
    printf("%s",recvbuf);
    int tok_len;
    char tok[MAX_MSG_TOKENS][MAX_MSG_LEN+1];
    char recvbuf2[MAX_MSG_LEN+1];
    bzero(&tok, sizeof(tok));
    bzero(&recvbuf2, sizeof(recvbuf2));

    get_msg(recvbuf, recvbuf2);
    tok_len = tokenize(recvbuf2,tok);
    int i = parse_msg(tok,tok_len,sendbuf,cur);
    printf("%d\n",i);
    return i;
    
}

