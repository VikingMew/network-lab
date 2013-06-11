#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include "csapp.h"
#include <string.h>
#include "rtlib.h"

#include "rt_network.h"
#include "irc.h"
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

int ircfd = -1;

int init_server() {

	int tcpsockfd;
	int rtsockfd;
	int max_fd;
	

	fd_set fds;

	int infd;

	tcpsockfd = open_tcp();
	rtsockfd = open_udp();
	printf("herererererererererer rt_network.c:38 tcpsockfd = %d, rtsockfd = %d\n", tcpsockfd, rtsockfd);


	while(1) {

		FD_ZERO(&fds);

		FD_SET(tcpsockfd, &fds);
		FD_SET(rtsockfd, &fds);
		if(ircfd > 0) {
			printf("rt_network.c:48 ircfd = %d\n", ircfd);
			FD_SET(ircfd, &fds);
		}

		max_fd = (tcpsockfd > rtsockfd? tcpsockfd : rtsockfd);
		max_fd = (max_fd > ircfd? max_fd: ircfd) + 1;

		// printf("herererererererererer rt_network.c:55\n");

		infd = Select(max_fd, &fds, 0, 0, 0);

		if(infd == 0) {
			continue;
		} 
		if(FD_ISSET(tcpsockfd, &fds)) {
			printf("rt_network: incoming tpc connect\n");
			ircfd = get_ircfd(tcpsockfd);
		} else if (infd == rtsockfd) {
			// do_routing(infd);
		} else if (FD_ISSET(ircfd, &fds)) {
			do_irc(ircfd);
		}

	}
}

int open_tcp() {
	return Open_listenfd(curr_node_config_entry->local_port);
}

int open_udp() {
	int sockfd, clilen, i;
	struct sockaddr_in serv_addr, cli_addr;
	struct packet incomingPacket;  // Temporary place to store incoming packet

	// setup the socket

	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sockfd < 0) {
		printf("error on socket() call");
		return -1;
	}

	// Setup the server address and bind to it
	memset(&serv_addr, '\0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(curr_node_config_entry->routing_port);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		printf("error binding socket\n");
		return -1;
	}
	return sockfd;
}

int get_ircfd(int fd) {

	int size;
	struct sockaddr_in caddr;
	return Accept(fd, (struct sockaddr*) &caddr, &size);
}