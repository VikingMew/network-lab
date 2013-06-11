#include "csapp.h"
#include "ircservice.h"
#include "sircd.h"
// #include "network.h"

extern struct fdlist *fdl;
extern u_long curr_nodeID;
int setmessage(int code,char command[MAX_MSG_LEN+1],
             char sendbuf[MAX_MSG_LEN+1],struct fdlist *cur)
{     //all list/who name list/ wont be in the function
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
    if(code == ERR_TOOMANYCHANNELS)
    {
        sprintf(sendbuf,":%s 405 %s %s :\r\n",
            SERVERNAME,cur->nickname,command);
    }
    if(code == RPL_ENDOFNAMES)   //'command' is channelname here!!!!
    {
        sprintf(sendbuf,":%s 366 %s %s :End of /NAMES list\r\n",
            SERVERNAME,cur->nickname,command);
    }
    if(code == RPL_ENDOFWHO)   //'command' is requestd name here!!!!
    {
        sprintf(sendbuf,":%s 315 %s %s :End of /WHO list\r\n",
            SERVERNAME,cur->nickname,command);
    }
    if(code == ERR_NOSUCHSERVER) //'command' is server name!!!!
    {
        sprintf(sendbuf,":%s 402 %s %s :No such server\r\n",
            SERVERNAME,cur->nickname,command);
    }
    if(code == RPL_LISTSTART)
    {
        sprintf(sendbuf,":%s 321 %s Channel :Users Name\r\n",
            SERVERNAME,cur->nickname);
    }
    if(code == RPL_LISTEND)
    {
        sprintf(sendbuf,":%s 323 %s :End of /LIST\r\n",
            SERVERNAME,cur->nickname);
    }
    if(code == ERR_NOSUCHNICK) //'command' is target name!!!!
    {
        sprintf(sendbuf,":%s 401 %s %s :No such nick/channel\r\n",
            SERVERNAME,cur->nickname,command);
    }
    if(code == ERR_NOTEXTTOSEND)
    {
        sprintf(sendbuf,":%s 412 %s :No text to send\r\n",
            SERVERNAME,cur->nickname);
    }
    if(code == ERR_NOTONCHANNEL) // 
    {
        sprintf(sendbuf,":%s 442 %s %s :You're not on that channel\r\n",
            SERVERNAME,cur->nickname,command);
    }
    if(code == ERR_NOSUCHCHANNEL)
    {
        sprintf(sendbuf,":%s 403 %s %s :No such channel\r\n",
            SERVERNAME,cur->nickname,command);
    }
    return 0;
}

int sendadduser(char nickname[MAX_MSG_LEN+1])
{
    char recvbuf[MAX_MSG_LEN+1];
    char sendbuf[MAX_MSG_LEN+1];
    bzero(&recvbuf, sizeof(recvbuf));
    bzero(&sendbuf, sizeof(sendbuf));
    sprintf(sendbuf,"ADDUSER %s\r\n",nickname);
    sendtodaemon(sendbuf,recvbuf);
    return 0;
}

int reqnexthop(char nickname[MAX_MSG_LEN+1])
{
    char recvbuf[MAX_MSG_LEN+1];
    char sendbuf[MAX_MSG_LEN+1];
    bzero(&recvbuf, sizeof(recvbuf));
    bzero(&sendbuf, sizeof(sendbuf));
    sprintf(sendbuf,"NEXTHOP %s\r\n",nickname);
    sendtodaemon(sendbuf,recvbuf);
                //TODO : parse returnd
    return 0;
}

int reqnexthops(char channel[MAX_MSG_LEN+1],int *nexthop,int *size)
{
    char recvbuf[MAX_MSG_LEN+1];
    char sendbuf[MAX_MSG_LEN+1];
    bzero(&recvbuf, sizeof(recvbuf));
    bzero(&sendbuf, sizeof(sendbuf));
    sprintf(sendbuf,"NEXTHOP %d %s\r\n",curr_nodeID,channel);
    sendtodaemon(sendbuf,recvbuf);
                //TODO : parse returnd
    return 0;
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
    if(strlen(cur->nickname) && strlen(cur->username))
    {
        //TODO: remove the old if 
        char recvbuf[MAX_MSG_LEN+1];
        char sendbuf[MAX_MSG_LEN+1];
        bzero(&recvbuf, sizeof(recvbuf));
        bzero(&sendbuf, sizeof(sendbuf));
        sprintf(sendbuf,"REMOVEUSER %s\r\n",cur->nickname);
        sendtodaemon(sendbuf,recvbuf);
    }
    else 
    {
        strcpy(cur->nickname,nickname);
        char buf[MAX_MSG_LEN + 1];
        sprintf(buf,":%s!%s NICK %s\r\n",nickname,SERVERNAME,nickname);
        boardcastbut(buf,cur->fd);
        if(strlen(cur->username))
        {
            sendadduser(cur->nickname);
            motd(cur);
        }
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
    strcpy(cur->realname,realname);
    if(strlen(cur->nickname))
    {
        sendadduser(cur->nickname);
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
    if(strlen(msg) == 0)
    {
        strcpy(sendmsg,cur->nickname);
    }
    else
    {
        strcpy(sendmsg,msg);
    }
    strcpy(nickname,cur->nickname);
    char recvbuf[MAX_MSG_LEN+1];
    char sendbuf[MAX_MSG_LEN+1];
    bzero(&recvbuf, sizeof(recvbuf));
    bzero(&sendbuf, sizeof(sendbuf));
    sprintf(sendbuf,"REMOVEUSER %s\r\n",nickname);
    sendtodaemon(sendbuf,recvbuf);
    //TODO: send quit channel msg to route
    return 0;
    sprintf(buf,":%s!%s QUIT %s\n",nickname,"localhost",sendmsg);
    boardcast(buf);
    return 0;
    // app_error("Not Implemented");
    // return -1;
}

int names(char channel[MAX_MSG_LEN+1],struct fdlist *cur)
{
    struct fdlist *now = fdl;
    char buf[MAX_MSG_LEN + 1];
    bzero(&buf, sizeof(buf));
    sprintf(buf,":%s 353 %s = %s :",SERVERNAME,
                cur->nickname,channel);
    while(now->next != 0)
    {
        if(!strcmp(now->next->channel,channel)) {
            if(!strlen(buf))
            {
                bzero(&buf, sizeof(buf));
                sprintf(buf,":%s 353 %s = %s :",SERVERNAME,
                            cur->nickname,channel);
            }
            if((strlen(buf) + strlen(now->nickname)) > MAX_MSG_LEN-5)
            {
                strcat(buf,"\r\n");
                sendmessage(buf,cur->fd);
                bzero(&buf, sizeof(buf));
                sprintf(buf,":%s 353 %s = %s :",SERVERNAME,
                            cur->nickname,channel);
            }
            strcat(buf,now->next->nickname);
        }
        now = now -> next;
    }
    if(strlen(buf))
    {
        strcat(buf,"\r\n");
        sendmessage(buf,cur->fd);
    }
    bzero(&buf, sizeof(buf));
    setmessage(RPL_ENDOFNAMES,channel,buf,cur);
    sendmessage(buf,cur->fd);
    return 0;
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
            channelname[next-current] = '\0';
            current = next + 1;
        }
        else 
        {
            strcpy(channelname, current);
            break;
        }
    }
    if(!strlen(channelname) || strlen(channelname) > 10) // up to 9 char
    {
        return ERR_NEEDMOREPARAMS;
    }
    if(channelname[0] != '&' && channelname[0] != '#')
    {
        return ERR_NEEDMOREPARAMS;
    }
    char oldchannel[MAX_MSG_LEN+1];
    if(strlen(cur->channel) && strcmp(cur->channel,channelname)) 
    {
        strcpy(oldchannel, cur->channel);
        part(oldchannel,cur);
    }
    bzero(cur->channel, sizeof(cur->channel));
    if(!existschannel(channelname))
    {
        printf("new channel!!! %s",channelname);
        char recv[MAX_MSG_LEN+1];
        char buf[MAX_MSG_LEN+1];
        bzero(&buf, sizeof(buf));
        sprintf(buf,"ADDCHAN %s\r\n",channelname);
        sendtodaemon(buf,recv);
    }
    else {
        printf("not new channel.. %s",channelname);

    }
    strcpy(cur->channel, channelname);
    char buf[MAX_MSG_LEN+1];
    sprintf(buf, ":%s JOIN %s\r\n", cur->nickname, channelname);
    boardcastchannel(buf,channelname);
    names(channelname,cur);
    return 0;
    // app_error("Not Implemented");
    // return -1;
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
int part(char channel[MAX_MSG_LEN+1],
         struct fdlist *cur)
{
    if(!strlen(channel))
    {
        return ERR_NEEDMOREPARAMS;
    }
    printf("in PART! %s\n",channel);
    char *current = channel;
    char channelname[MAX_MSG_LEN+1];
    bzero(&channelname,sizeof(channelname));
    while(strlen(channelname) == 0)
    {
        bzero(&channelname,sizeof(channelname));
        char *next = strchr(current, ' ');
        if (next)
        {
            memcpy(channelname, current, next-current); // get the parameter
            channelname[next-current] = '\0';
            current = next + 1;
        }
        else 
        {
            strcpy(channelname, current);
        }
        if(!strlen(channelname) || strlen(channelname) > 10) // up to 9 char
        {
            return ERR_NEEDMOREPARAMS;
        }
        if(channelname[0] != '&' && channelname[0] != '#')
        {
            return ERR_NEEDMOREPARAMS;
        }
        char oldchannel[MAX_MSG_LEN+1];
        char buf[MAX_MSG_LEN+1];
        printf("%s\n%s\n\n",channelname,cur->channel);

        if(strlen(cur->channel) && !strcmp(cur-> channel, channelname)) 
        {
            bzero(&buf, sizeof(buf));
            sprintf(buf,":%s!%s@localhost QUIT :%s\r\n",cur->nickname,cur->username,channelname);
            boardcastchannel(buf,channelname);
            bzero(cur->channel, sizeof(cur->channel));
            printf("\n\n\n %d \n\n\n",existschannel(channelname));
            if(!existschannel(channelname))
            {
                printf("last channel!! %s",channelname);
                char recv[MAX_MSG_LEN+1];
                bzero(&buf, sizeof(buf));
                sprintf(buf,"REMOVECHAN %s\r\n",channelname);
                sendtodaemon(buf,recv);
            }
            else
            {
                printf("not last channel!! %s",channelname);

            }
        }
        else
        {
            bzero(&buf, sizeof(buf));
            setmessage(ERR_NOTONCHANNEL,channelname,buf,cur);
            sendmessage(buf,cur->fd);
        }
    }

    return 0;
    // app_error("Not Implemented");
    // return -1;
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
int list(char channels[MAX_MSG_LEN+1], //ignore by 15-441
         char server[MAX_MSG_LEN+1],   //ignore by 15-441
         struct fdlist *cur)
{
    //"<channel> <# visible> :<topic>"
    char buf[MAX_MSG_LEN+1];
    bzero(&buf, sizeof(buf));
    setmessage(RPL_LISTSTART,"",buf,cur);
    sendmessage(buf,cur->fd);
    char channellist[200][MAX_MSG_LEN+1];
    int usercount[200];
    int channelcount = 0;
    bzero(&channellist, sizeof(channellist));
    bzero(&usercount, sizeof(usercount));
    struct fdlist *now = fdl;
    while(now->next != 0)
    {
        int inlist = 0;
        int i;
        for(i = 0;i < channelcount; i++)
        {
            if(!strcmp(channellist[i],now->next->channel))
            {
                inlist = 1;
                usercount[i] += 1;
                break;
            }
        }
        if(!inlist)
        {
            strcpy(channellist[channelcount],now->next->channel);
            usercount[channelcount] += 1;
            channelcount++;
        }
        now = now -> next;
    }
    int i = 0;
    for(i = 0;i < channelcount;i++) 
    {
        bzero(&buf, sizeof(buf));
        sprintf(buf,":%s 322 %s %s %d\r\n",
                    SERVERNAME,
                    cur->nickname,
                    channellist[i],
                    usercount[i]);
        sendmessage(buf,cur->fd);
    }
    bzero(&buf, sizeof(buf));
    setmessage(RPL_LISTEND,"",buf,cur);
    sendmessage(buf,cur->fd);
    return 0;
    // app_error("Not Implemented");
    // return -1;
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
int privmsg(char target[MAX_MSG_LEN+1],
            char message[MAX_MSG_LEN+1],
            struct fdlist *cur)
{
    printf("privmsg!!%s!\n",message);
    if(!strlen(message))
    {
        return ERR_NOTEXTTOSEND;
    }
    int issent = 0;
    // char targetlist[200][MAX_MSG_LEN+1];
    char buf[MAX_MSG_LEN+1];
    int i = 0;
    char* now = message;
    char* p;
    bzero(&buf, sizeof(buf));
    while(p = strsep(&now,","))
    {
        int tmpsent = 0;
        if(p[0] != '&' && p[0] != '#') 
        {
            struct fdlist *node = findbynickname(p);
            if(node != 0)
            {
                bzero(&buf, sizeof(buf));
                sprintf(buf,":%s PRIVMSG %s :%s\r\n",
                            cur->nickname,
                            node->nickname,
                            message);
                sendmessage(buf,node->fd);
                tmpsent = 1;
            }
            else
            {
                int nexthop,ret;
                nexthop = reqnexthop(p);
                if(nexthop > 0)
                {
                    sendtonode(nexthop,send);
                    tmpsent = 1;
                }
            }
        }
        else
        {
            bzero(&buf, sizeof(buf));
            sprintf(buf,":%s PRIVMSG %s :%s\r\n",
                cur->nickname,
                now,
                p);
            tmpsent += (boardcastchannel(buf,p));
            //send to other hops
            int size;
            int* nexthop;
            reqnexthops(p,nexthop,&size);
            //TODO : send via route
            //TODO : request route
            //TODO : forward message
        }
        if(!tmpsent)
        {
            bzero(&buf, sizeof(buf));
            setmessage(ERR_NOSUCHNICK,"",buf,cur);
            sendmessage(buf,cur->fd);
        }
        now = p;
    }
    return 0;
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
int who(char request[MAX_MSG_LEN+1],
        char sign[MAX_MSG_LEN+1], //ignored by 15-441
        struct fdlist *cur)
{
    // char channelist[200][MAX_MSG_LEN+1]; //200 is the maxmium to the channel 
    // char requestlist[200][MAX_MSG_LEN+1];
    // bzero(&channellist,sizeof(channellist));
    char buf[MAX_MSG_LEN + 1];
    if(!strlen(request))
    {
        struct fdlist *now = fdl;
        while(now->next != 0)
        {
            bzero(&buf, sizeof(buf));
            sprintf(buf,":%s 352 %s %s %s %s %s %s H :0 %s\r\n",
                        SERVERNAME,
                        cur->nickname,
                        now->next->channel,
                        now->next->username,
                        now->next->hostname,
                        now->next->servername,
                        now->next->nickname,
                        now->next->realname);
            sendmessage(buf,cur->fd);
            now = now -> next;
        }
        bzero(&buf, sizeof(buf));
        setmessage(RPL_ENDOFWHO,request,buf,cur);
        sendmessage(buf,cur->fd);
    }
    else
    {
        struct fdlist *now = fdl;
        while(now->next != 0)
        {
            if(!strcmp(request,now->next->channel))
            {
            bzero(&buf, sizeof(buf));
            sprintf(buf,":%s 352 %s %s %s %s %s %s H :0 %s\r\n",
                        SERVERNAME,
                        cur->nickname,
                        now->next->channel,
                        now->next->username,
                        now->next->hostname,
                        now->next->servername,
                        now->next->nickname,
                        now->next->realname);
            sendmessage(buf,cur->fd);
            }
            now = now -> next;
        }
        bzero(&buf, sizeof(buf));
        setmessage(RPL_ENDOFWHO,request,buf,cur);
        sendmessage(buf,cur->fd);
    }
    return 0;
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
        // i = RPL_CLOSING;
        quit("",cur);
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
    if(!strcmp(token[0],"WHO"))
    {
        i = who(token[1],token[2],cur);
    }
    if(!strcmp(token[0],"LIST"))
    {
        i = list(token[1],token[2],cur);
    }
    if(!strcmp(token[0],"PRIVMSG"))
    {
        i = privmsg(token[1],token[2],cur);
    }
    if(!strcmp(token[0],"PART"))
    {
        i = part(token[1],cur);
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

