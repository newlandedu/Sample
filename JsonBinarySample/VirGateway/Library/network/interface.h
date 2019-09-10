
#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NETWORK_INTERFACE_NAME_MAX		30
#define NETWORK_INTERFACE_IP_ADDR_MAX		20
#define NET_INTERFACE_NUM_MAX	5


struct net_interface_inf{
	char if_name[NETWORK_INTERFACE_NAME_MAX];
	char if_ip[NETWORK_INTERFACE_IP_ADDR_MAX];
};

int get_all_interface(struct net_interface_inf* if_inf, int* if_num);
int get_ip_by_interface_name(char* if_name, char* if_ip);
void show_interface_info(struct net_interface_inf* if_inf);

#endif

