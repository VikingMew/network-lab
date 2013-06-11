#include "srouted.h"

#define MAX_ENTRIES 6

struct packet {
	int seq_num;
	int sourceNode;
	int numEntries;
	char entries[MAX_ENTRIES][16];
};

int init_server();