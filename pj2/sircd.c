/* To compile: gcc sircd.c rtlib.c rtgrading.c csapp.c -lpthread -osircd */

#include "rtlib.h"
#include "rtgrading.h"
#include "csapp.h"
#include <stdlib.h>

// #include "sircd.h"
#include "ircservice.h"
// #include "network.h"


/* Global variables */
u_long curr_nodeID;
rt_config_file_t   curr_node_config_file;  /* The config_file  for this node */
rt_config_entry_t *curr_node_config_entry; /* The config_entry for this node */

/* Function prototypes */
void init_node( int argc, char *argv[] );
size_t get_msg( char *buf, char *msg );
// int tokenize( char const *in_buf, char tokens[MAX_MSG_TOKENS][MAX_MSG_LEN+1] );

/* Main */
int main( int argc, char *argv[] )
{
    init_node( argc, argv );
    printf( "I am node %lu and I listen on port %d for new users\n", curr_nodeID, curr_node_config_entry->irc_port );
    init_server();
    return 0;
}

/*
 * void init_node( int argc, char *argv[] )
 *
 * Takes care of initializing a node for an IRC server
 * from the given command line arguments
 */
void init_node( int argc, char *argv[] )
{
    int i;

    if( argc < 3 )
    {
        printf( "%s <nodeID> <config file>\n", argv[0] );
        exit( 0 );
    }

    /* Parse nodeID */
    curr_nodeID = atol( argv[1] );

    /* Store  */
    rt_parse_config_file(argv[0], &curr_node_config_file, argv[2] );

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
}
