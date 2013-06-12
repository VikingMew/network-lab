#include "routing.h"

/* Global variables */
extern u_long curr_nodeID;
extern rt_config_file_t   curr_node_config_file;  /* The config_file  for this node */
extern rt_config_entry_t *curr_node_config_entry; /* The config_entry for this node */

/* ===== OSPF OPTIONS ===== */
extern unsigned long curr_node_advertisement_cycle_time; /* -a advertisement cycle */
extern unsigned long curr_node_neighbor_timeout;         /* -n timeout for dead neighbors */
extern unsigned long curr_node_retransmission_timeout;   /* -r timeout for retransmission */
extern unsigned long curr_node_lsa_timeout;              /* -t timeout to expire an LSA */

struct NEIGHBOUR_INFO
{
	int NodeID;	
	struct sockaddr_in Address;	
	time_t LastReceived;
	int IsDown;						
};

struct LSA_LIST
{
	int LSACount;
	struct {
		int NodeID;
		char *LSABuffer;
		int Acked[MAX_NEIGHBOURS];
	} LSAs[MAX_NODES];

	int CurrentNodeID;									
	int NeighbourCount;									
	struct NEIGHBOUR_INFO Neighbours[MAX_NEIGHBOURS];	

	int ShortestPathReady;								
	struct {
		int Cost;										
		int NextHop;									
	} ShortestPath[MAX_NODES][MAX_NODES];				
};

struct SEND_TARGET			
{
	int LSAIndex;			
	int NeighbourIndex;		
	int SeqNumber;			
	int NeedAck;			
};
											
struct LSA_LIST LSACache;										

struct LOCAL_LIST LocalIRCList;			


void do_routing(int fd) {

}

void InitializeLSA()
{
	int i;

	LSACache.LSACount = 1;
	LSACache.LSAs[0].LSABuffer = malloc(24);
	bzero(LSACache.LSAs[0].LSABuffer, 24);
	LSACache.LSAs[0].NodeID = curr_nodeID;
	for (i = 0; i < curr_node_config_file.size; i++)
		LSACache.LSAs[0].Acked[i] = 0;
	LSACache.CurrentNodeID = curr_nodeID;
	LSACache.NeighbourCount = curr_node_config_file.size;
	for (i = 0; i < curr_node_config_file.size; i++) {
		LSACache.Neighbours[i].NodeID = curr_node_config_file.entries[i].nodeID;
		LSACache.Neighbours[i].Address.sin_addr.s_addr = htonl(curr_node_config_file.entries[i].ipaddr);
		LSACache.Neighbours[i].Address.sin_port = htons(curr_node_config_file.entries[i].routing_port);
		LSACache.Neighbours[i].LastReceived = time(NULL);
		LSACache.Neighbours[i].IsDown = 0;
	}
	LSACache.ShortestPathReady = 0;


	LocalIRCList.UserList.Count = 0;
	LocalIRCList.UserList.header = NULL;
	LocalIRCList.ChannelList.Count = 0;
	LocalIRCList.ChannelList.header = NULL;
}