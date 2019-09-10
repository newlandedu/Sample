
#include "interface.h"

#if 0
int main(int argc, char *argv[])
{
	struct ifaddrs *ifaddr, *ifa;
	int family, s;
	char host[NI_MAXHOST];

	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		exit(EXIT_FAILURE);
	}

	/* Walk through linked list, maintaining head pointer so we
	   can free list later */

	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr == NULL)
			continue;

		family = ifa->ifa_addr->sa_family;

		/* Display interface name and family (including symbolic
		   form of the latter for the common families) */

		printf("%s	address family: %d%s\n",
				ifa->ifa_name, family,
				(family == AF_PACKET) ? " (AF_PACKET)" :
				(family == AF_INET) ?	" (AF_INET)" :
				(family == AF_INET6) ?	" (AF_INET6)" : "");

		/* For an AF_INET* interface address, display the address */

		if (family == AF_INET || family == AF_INET6) {
			s = getnameinfo(ifa->ifa_addr,
					(family == AF_INET) ? sizeof(struct sockaddr_in) :
										  sizeof(struct sockaddr_in6),
					host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
			if (s != 0) {
				printf("getnameinfo() failed: %s\n", gai_strerror(s));
				exit(EXIT_FAILURE);
			}
			printf("\taddress: <%s>\n", host);
		}
	}

	freeifaddrs(ifaddr);
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	struct net_interface_inf interface[10];
	int req = 10;
	int ret, i;

	memset(interface, 0, sizeof(interface));
	ret = get_interface(interface, &req);
	if(ret == 0){
		for(i = 0; i < req; i++)
			show_interface_info(&interface[i]);
	}
	
	return 0;
}


#endif
/* search for AF_INET interface address
   if_inf, store the interface name
   if_num, initially containing the num of interface that wanted, 
   		and modified on return to indicate the actual num of interface returned.
   return = 0 sucess
   return -1, error */
int get_all_interface(struct net_interface_inf* if_inf, int* if_num)
{
	struct ifaddrs *ifaddr, *ifa;
	struct sockaddr_in *sock_addr;
	int req = *if_num;
	char*ip;

	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		return -1;
	}

	*if_num = 0;

	/* Walk through linked list, maintaining head pointer so we can free list later */
	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr == NULL)
			continue;
		sock_addr = (struct sockaddr_in *)ifa->ifa_addr;
		/* For an AF_INET(AF_INET6 not support) interface address, display the address */
		if (sock_addr->sin_family == AF_INET) {
			ip = inet_ntoa(sock_addr->sin_addr);
			sprintf(if_inf[*if_num].if_ip, "%s", ip);
			sprintf(if_inf[*if_num].if_name, "%s", ifa->ifa_name);
			(*if_num)++;
			if(*if_num == req)
				break;
		}
	}

	freeifaddrs(ifaddr);
	return 0;
}

/* get network card IP address
   if_name, network card interface name
   if_ip, return the network card IP address
   return = 0 sucess
   return -1, error */
int get_ip_by_interface_name(char* if_name, char* if_ip)
{
	struct net_interface_inf interface[NET_INTERFACE_NUM_MAX];
	int req = NET_INTERFACE_NUM_MAX;
	int ret, i;

	memset(interface, 0, sizeof(interface));
	ret = get_all_interface(interface, &req);
	if(ret < 0){
		return -1;
	}

	for(i = 0; i < req; i++){
		//show_interface_info(&interface[i]);
		//printf("find:%s\n", if_name);
		if(strcmp(if_name, interface[i].if_name) == 0){
			memcpy(if_ip, interface[i].if_ip, strlen(interface[i].if_ip));
			return 0;
		}
	}

	return -1;
}

void show_interface_info(struct net_interface_inf* if_inf)
{
	if(if_inf){
		printf("interface:%s  ip:%s\n", if_inf->if_name, if_inf->if_ip);
	}
}


