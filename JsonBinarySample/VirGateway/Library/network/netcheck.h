#ifndef _NET_CHECK_H_
#define _NET_CHECK_H_
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>




#define MAX_WAIT_TIME   1
#define MAX_NO_PACKETS  1
#define ICMP_HEADSIZE 8 
#define PACKET_SIZE     4096



//函数定义
int ping(char *ip);
int reboot();



#endif

