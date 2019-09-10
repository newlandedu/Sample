/**********************************************************
	Modify History: qiuzhb make, 2016-8-21
**********************************************************/


#ifndef TCPSOCK_H_
#define TCPSOCK_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <net/if.h>



#define MAX_CONN	10
#define ERROR_PROCESS	1

//#define TCP_SOCKET_DEBUG
#ifdef TCP_SOCKET_DEBUG
#define TCP_SOCKET_DBG(fmt, args...)	printf("TCP SOCKET DEBUG[%s][%d]: "fmt, __FILE__, __LINE__, ##args)
#else
#define TCP_SOCKET_DBG(fmt, args...)
#endif

//#define TCP_SOCKET_DEERR
#ifdef TCP_SOCKET_DEERR
#define TCP_SOCKET_ERR(fmt, args...)	printf("TCP SOCKET ERROR[%s][%d]: "fmt, __FILE__, __LINE__, ##args)
#else
#define TCP_SOCKET_ERR(fmt, args...)
#endif



typedef enum
{
	SELECT_READ,
	SELECT_WRITE,
	SELECT_EXCEPTION,
	SELECT_FOR_SLEEP
}SELECT_OPT;


typedef void(*error_process_callback) (int sock, int error_num);

int open_client_port(char* if_name, unsigned short client_port);
int open_server_port(char* if_name, unsigned short server_port);
int set_socket_non_blocking(int sock);
int connection_server(int client_sock, char* server_num_and_dot_IPv4, unsigned short server_port);
int accept_client_connection(int server_socket, 
		char *client_num_and_dot_IPv4, unsigned short *client_port);
int close_connection(int sock);
int shutdown_connection(int sock);

void error_process(int sock, int error_num);
int send_packet(int sock, void *buf, int len, int is_no_wait);
int receive_packet(int sock, void *buf, int len, int is_no_wait);
int select_socket(int sock, int ms_timeout, SELECT_OPT opt);
int clear_sock_receive_buffer(int sock);

#endif // #ifndef TCPSOCK_H_
