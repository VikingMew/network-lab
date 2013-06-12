#include <time.h>
#include <limits.h>
#include <search.h>
#include "csapp.h"
#include "rtgrading.h"
#include "rtlib.h"

#define	MAX_NODES		256
#define	MAX_NEIGHBOURS	MAX_CONFIG_FILE_LINES

typedef struct T_NODE
{
	struct T_NODE *next;
	char *data;
} node_t;

typedef struct T_LIST
{
	int Count;
	struct T_NODE *header;
} list_t;

struct LOCAL_LIST
{
	struct T_LIST UserList;
	struct T_LIST ChannelList;
};

typedef struct
{
	int NodeID;
	int Distance;
} nexthop_t;


typedef struct CHANHOPNODE_TAG
{
	int NodeID;
	struct CHANHOPNODE_TAG *next;
} chanhopnode_t;

typedef struct CHANHOPS_TAG
{
	int SourceNode;
	const char *ChannelName;
	chanhopnode_t *NextHops;
	struct CHANHOPS_TAG *next;
} chanhops_t;

typedef struct
{
	int Count;
	chanhops_t *FirstChannel;
} chantable_t;

extern struct LOCAL_LIST LocalIRCList;


void InitializeLSA();
void do_routing();


