/* Sroute Definination */
#include "srouted.h"

/* To compile: gcc sircd.c rtlib.c rtgrading.c csapp.c -lpthread -osircd */
#include "rtlib.h"
#include "rtgrading.h"
#include "csapp.h"
#include <stdlib.h>

/* Global variables */
u_long curr_nodeID;
rt_config_file_t   curr_node_config_file;  /* The config_file  for this node */
rt_config_entry_t *curr_node_config_entry; /* The config_entry for this node */

/* ===== OSPF OPTIONS ===== */
unsigned long curr_node_advertisement_cycle_time; /* -a advertisement cycle */
unsigned long curr_node_neighbor_timeout;         /* -n timeout for dead neighbors */
unsigned long curr_node_retransmission_timeout;   /* -r timeout for retransmission */
unsigned long curr_node_lsa_timeout;              /* -t timeout to expire an LSA */

/* init deamon */
void init_daemon(int argc, char **argv);

/* Main */
int main( int argc, char *argv[] )
{
    rt_init( argc, argv );
    init_daemon( argc, argv); 
    printf( "I am route node %lu and I listen on port %d for new users\n", curr_nodeID, curr_node_config_entry->local_port );
    init_server();
    return 0;
	
}

void init_daemon(int argc, char **argv) {
    int i;

    if( argc < 3 )
    {
        printf( "%s -i <nodeID> -c <config file> -a <advertisement cycle time(seconds)> [-n|r|t]\n", argv[0] );
        exit( 0 );
    }
	
	

    /* Parse arguments */
	
	rt_args_t args;
	rt_parse_command_line(&args, argc, argv);
	
	
	curr_nodeID = args.nodeID;
	curr_node_config_file = args.config_file;
	curr_node_advertisement_cycle_time = args.advertisement_cycle_time;
	curr_node_neighbor_timeout = args.neighbor_timeout;
	curr_node_retransmission_timeout = args.retransmission_timeout;
	curr_node_lsa_timeout = args.lsa_timeout;

    /* Get config file for this node */
    for( i = 0; i < curr_node_config_file.size; ++i )
        if( curr_node_config_file.entries[i].nodeID == curr_nodeID )
             curr_node_config_entry = &curr_node_config_file.entries[i];

    /* Check to see if nodeID is valid */
    if( !curr_node_config_entry )
    {
        printf( "Invalid NodeID\n" );
        exit(1);
    }
	return;
}



