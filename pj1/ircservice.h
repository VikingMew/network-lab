#define MAX_MSG_TOKENS 10
#define MAX_MSG_LEN 512

// int nick(char* nickname);
// int user();
// int quit();
// int join();
// int part();
// int list();
// int privmsg();
// int who();
int parse_msg(char token[MAX_MSG_TOKENS][MAX_MSG_LEN+1],int length);