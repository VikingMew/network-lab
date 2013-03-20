#include "csapp.h"
#include "ircservice.h"
#include "sircd.h"

/* int setnickname(char* nickname)
 *
 * set the nickname to the curclient
 * Command:   NICK
 * Parameters:     <nickname> [ <hopcount> ]
 * error ERR_NICKNAMEINUSE 
 * return -1 when ERR_NICKNAMEINUSE 
 * return 0 while success
 */
int nick(char* nickname)
{
    app_error("Not Implemented");
    return -1;
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
 * return 0 
 */
int quit()
{
    app_error("Not Implemented");
    return -1;
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
int parse_msg(char token[MAX_MSG_TOKENS][MAX_MSG_LEN+1],int length)
{
    app_error("Not Implemented");
    return -1;
}
