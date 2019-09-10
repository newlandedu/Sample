/**********************************************************
	Modify History: qiuzhb make, 2016-8-21
**********************************************************/


#include "tcpsock.h"
#include "interface.h"


/* create a tcp socket with server mode
   if_name(opt), interface name
   client_port(opt)
   return > 0(socket fd), sucess
   return -1, error */
int open_client_port(char* if_name, unsigned short client_port)
{
	int sock;
	struct sockaddr_in client_addr;
	int opt_val = 1;
	in_addr_t addr;
	char ip_addr[30];
	int ret;
	struct ifreq ifr;  

	//create tcp ssocket
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		TCP_SOCKET_ERR("create client tcp socket error\n");
		return -1;
    }

	// 一般不会立即关闭而经历TIME_WAIT的过程 后想继续重用该socket
	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)) == -1){
		TCP_SOCKET_ERR("setsockopt client tcp socket error\n");
		goto socket_exit;
	}

	/* if not bind the interface, you can use other interface to send package */
	#if 1
	//BIND interface, requires root privileges to run
	if(if_name){
		memset(&ifr, 0x00, sizeof(ifr));  
		strncpy(ifr.ifr_name, if_name, strlen(if_name));
										//将套接字绑定在特定的设备上
		if(setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, (char *)&ifr, sizeof(ifr)) < 0){
			TCP_SOCKET_ERR("fail to set SO_BINDTODEVICE attribution. error:%s, %d\n", strerror(errno), errno);
			goto socket_exit;
		}
	}
	#endif

	//bind port
	if(client_port > 0)
	{
			if(if_name)
			{
				memset(ip_addr, 0, sizeof(ip_addr));
				ret = get_ip_by_interface_name(if_name, ip_addr);
				if(ret < 0){
					TCP_SOCKET_ERR("get_ip_by_interface_name error, %s not exist\n", if_name);
					goto socket_exit;
				}
				addr = inet_addr(ip_addr);
				if(addr == -1){
					TCP_SOCKET_ERR("ip address error:%s\n", ip_addr);
					return -1;
				}
			}
			else
			{
				addr = htonl(INADDR_ANY);
			}
			//bind
			memset(&client_addr, 0, sizeof(client_addr));
			client_addr.sin_family = AF_INET;
			client_addr.sin_addr.s_addr = addr;  /* any interface */
			client_addr.sin_port = htons(client_port);
		    if(bind(sock, (struct sockaddr*)&client_addr, sizeof(client_addr)) == -1)
			{
				TCP_SOCKET_ERR("bind client tcp socket error\n");
				goto socket_exit;
		    }
	}
	return sock;

socket_exit:
	close(sock);
	return -1;
	
}

/* create a tcp socket with server mode
   num_and_dot_IPv4(opt)
   return > 0(socket fd), sucess
   return -1, error */
int open_server_port(char* if_name, unsigned short server_port)
{
	int sock;
	struct sockaddr_in serv_addr;
	int opt_val = 1;
	in_addr_t addr;
	char ip_addr[30];
	int ret;
	struct ifreq ifr;  

	//create tcp ssocket
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		TCP_SOCKET_ERR("create service tcp socket error\n");
		return -1;
    }

	//set reuse of local addresses
	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)) == -1){
		TCP_SOCKET_ERR("setsockopt service tcp socket error\n");
		goto socket_exit;
	}

	/* if not bind the interface, you can use other interface to send package */
	#if 1
	//BIND interface, requires root privileges to run
	if(if_name){
		memset(&ifr, 0x00, sizeof(ifr));  
		strncpy(ifr.ifr_name, if_name, strlen(if_name));	
		if(setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, (char *)&ifr, sizeof(ifr)) < 0){
			TCP_SOCKET_ERR("fail to set SO_BINDTODEVICE attribution. error:%s, %d\n", strerror(errno), errno);
			goto socket_exit;
		}
	}
	#endif


	if(if_name){
		memset(ip_addr, 0, sizeof(ip_addr));
		ret = get_ip_by_interface_name(if_name, ip_addr);
		if(ret < 0){
			TCP_SOCKET_ERR("get_ip_by_interface_name error, %s not exist\n", if_name);
			goto socket_exit;
		}
		addr = inet_addr(ip_addr);
		if(addr == -1){
			TCP_SOCKET_ERR("ip address error:%s\n", ip_addr);
			goto socket_exit;
		}
	}else{
		addr = htonl(INADDR_ANY);
	}

	//bind
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = addr;  /* any interface */
	serv_addr.sin_port = htons(server_port);
    if(bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
		TCP_SOCKET_ERR("bind service tcp socket error\n");
		goto socket_exit;
    }

	//listen
    if(listen(sock, MAX_CONN) == -1){
		TCP_SOCKET_ERR("listen service tcp socket error\n");
		goto socket_exit;
    }

	printf("start newlab tcp server, ip:%s, port:%d\n", ip_addr, server_port);

	return sock;

socket_exit:
	close(sock);
	return -1;
	
}

/* set socket non-blocking
   return 0, sucess
   return -1, error */
int set_socket_non_blocking(int sock)
{
	int flags;

	if((flags = fcntl(sock, F_GETFL, 0)) == -1){
		TCP_SOCKET_ERR("Get the file access mode and the file status flags error\n");
		return -1;
	}

	if(fcntl(sock, F_SETFL, flags | O_NONBLOCK) == -1){
		TCP_SOCKET_ERR("set O_NONBLOCK flags error\n");
		return -1;
	}
	return 0;
}

/* connection server
   err_call_bk(opt)
   return < 0, error
   return = 0, sucess*/
int connection_server(int client_sock, char* server_num_and_dot_IPv4, 
	unsigned short server_port)
{
	int ret;
	struct sockaddr_in serv_addr;
	in_addr_t addr;

	if(client_sock <= 0){
		TCP_SOCKET_ERR("error client_sock:%d\n", client_sock);
		return -1;
	}
	if(server_port < 0){
		TCP_SOCKET_ERR("error server_port:%d\n", server_port);
		return -1;
	}
	if(server_num_and_dot_IPv4){
		addr = inet_addr(server_num_and_dot_IPv4);
		if(addr == -1){
			TCP_SOCKET_ERR("ip address error:%s\n", server_num_and_dot_IPv4);
			return -1;
		}
	}else{
		TCP_SOCKET_ERR("server_num_and_dot_IPv4 error\n");
		return -1;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(server_port);
	serv_addr.sin_addr.s_addr = addr;
	TCP_SOCKET_DBG("link ip:%s port:%d ...\r\n",server_num_and_dot_IPv4,server_port);
	if((ret = connect(client_sock, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in))) < 0){
		TCP_SOCKET_ERR("connect error(%d:%s)\n", ret, strerror(errno));
		return ret;
	}
	TCP_SOCKET_DBG("link success\r\n");
	return 0;
}

/* do accept 
return -1, error
return > 0,(socket) sucess*/
int accept_client_connection(int server_socket, 
		char *client_num_and_dot_IPv4, unsigned short *client_port)
{
	int client_sock, len;
    struct sockaddr_in addr;
	char* ip;

	len = sizeof(addr);
	memset(&addr, 0, sizeof(addr));


	if((client_sock = accept(server_socket, (struct sockaddr*)&addr, (socklen_t*)&len)) == -1){
		TCP_SOCKET_ERR("accept client connection error\n");
 		return -1;  /* Accept error. */
	}



	if(client_num_and_dot_IPv4){
		ip = inet_ntoa(addr.sin_addr);
		sprintf(client_num_and_dot_IPv4, "%s", ip);
	}
	if(client_port){
		*client_port = ntohs(addr.sin_port);
	}
	
	return client_sock;
}		
	  
// do close
int close_connection(int sock)
{
    if(close(sock) == -1){
		TCP_SOCKET_ERR("close socket error\n");
		return -1;
    }
	return 0;
}

int shutdown_connection(int sock)
{
	if(shutdown(sock,0) == -1){
		TCP_SOCKET_ERR("shutdown socket error\n");
		return -1;
    }
	return 0;
}

/* process errno */
void error_process(int sock, int error_num)
{
#ifdef ERROR_PROCESS
	switch(error_num){
		case EBADF: //An invalid file descriptor was given in one of the sets. 
					//Perhaps a file descriptor that was already closed
			//close_connection(sock);
			TCP_SOCKET_ERR("file descriptor that was already closed\n");
			break;
		case ECONNRESET:
			//close_connection(sock);
			TCP_SOCKET_ERR("Connection reset by peer.\n");
			break;
		case EDESTADDRREQ:
			TCP_SOCKET_ERR("The socket is not connection-mode, and no peer address is set.\n");
			break;
		case EINTR: //A signal was caught;
			TCP_SOCKET_ERR("A signal was caught\n");
			break;
		case EINVAL:
			TCP_SOCKET_ERR("nfds is negative or the value contained within timeout is invalid.\n");
			break;
		case EWOULDBLOCK:
			TCP_SOCKET_ERR("The socket is marked nonblocking and the requested operation would block.\n");
			break;
		case ENOTCONN:
			TCP_SOCKET_ERR("The socket is not connected, and no target has been given.\n");
			break;
		case ENOTSOCK:
			TCP_SOCKET_ERR("he argument sockfd is not a socket.\n");
			break;
		case EOPNOTSUPP:
			TCP_SOCKET_ERR("Some bit in the flags argument is inappropriate for the socket type.\n");
			break;
		case ECONNREFUSED:
			TCP_SOCKET_ERR("A remote host refused to allow the network connection (typically because it is not running the requested service).\n");
			break;
		default:
			break;
	}
#endif
}

/* return < 0, error
   return > 0,(send data len) sucess*/
int send_packet(int sock, void *buf, int len, int is_no_wait)
{
	int ret;
	int flag;
	
	if(sock <= 0){
		TCP_SOCKET_ERR("error sock:%d\n", sock);
		return -1;
	}
	if(buf == NULL){
		TCP_SOCKET_ERR("error buf:%lx\n", (unsigned long)buf);
		return -1;
	}
	if(len < 0){
		TCP_SOCKET_ERR("error len:%d\n", len);
		return -1;
	}

	//set blocking
	if(is_no_wait > 0){
		flag = MSG_DONTWAIT;
	}else{
		flag = 0;
	}

	if((ret = send(sock, buf, len, flag)) < 0){
		TCP_SOCKET_ERR("send packet error(%d:%s)\n", ret, strerror(errno));
		return ret;
	 }

	return ret;
}

/* is_no_wait(opt, if > 0 wait until message received)
   err_call_bk(opt)
   return 0, connect has been close by remote side
   return < 0, error
   return > 0,(received data len) sucess*/
int receive_packet(int sock, void *buf, int len, int is_no_wait)
{
	int ret;
	int flag;
	
	if(sock <= 0){
		TCP_SOCKET_ERR("error sock:%d\n", sock);
		return -1;
	}
	if(buf == NULL){
		TCP_SOCKET_ERR("error buf:%lx\n", (unsigned long)buf);
		return -1;
	}
	if(len < 0){
		TCP_SOCKET_ERR("error len:%d\n", len);
		return -1;
	}

	//set blocking
	if(is_no_wait > 0){
		flag = MSG_DONTWAIT;
	}else{
		flag = 0;
	}

	if((ret = recv(sock, buf, len, flag)) < 0){
		TCP_SOCKET_ERR("send packet error(%d:%s)\n", ret, strerror(errno));
		return ret;
	}else if(ret == 0){
		TCP_SOCKET_ERR("the connect has been close by remote side.(%d:%s)\n", ret, strerror(errno));
		return 0;
	}else{
		return ret;
	}
}



/* do select 
opt, for select option
return 0, timeout
return < 0, error
return > 0, sucess
*/
int select_socket(int sock, int ms_timeout, SELECT_OPT opt)
{
    int ret;
    struct timeval *time_out, timer;
    fd_set rfds, wfds, efds;
	fd_set *p_rfds = NULL, *p_wfds = NULL, *p_efds = NULL;

	if(sock <= 0){
		TCP_SOCKET_ERR("error sock:%d\n", sock);
		return -1;
	}

	if(ms_timeout >= 0){
	    timer.tv_sec = ms_timeout / 1000; 
	    timer.tv_usec = (ms_timeout % 1000) * 1000;
		time_out = & timer;
	}else{
		time_out = NULL;
	}

	/**** clear the fdset then set the socket in the fdset for selection ********/
	switch(opt){
		case SELECT_READ:
			FD_ZERO(&rfds);
			FD_SET(sock, &rfds);
			p_rfds = &rfds;
			break;
		case SELECT_WRITE:
			FD_ZERO(&wfds);
			FD_SET(sock, &wfds);
			p_wfds = &wfds;
			break;
		case SELECT_EXCEPTION:
			FD_ZERO(&efds);
			FD_SET(sock, &efds);
			p_efds = &efds;
			break;
		case SELECT_FOR_SLEEP:
			//do nothing
			break;
		default:
			return -1;
	}

	//printf("start\n");
    ret = select(sock+1, p_rfds, p_wfds, p_efds, time_out);
	//printf("end\n");
	if(ret < 0){
		/**** select error ******/
		TCP_SOCKET_ERR("select error(%d:%s)\n", ret, strerror(errno));
		return ret;
	}else if(ret == 0){
		if(opt == SELECT_FOR_SLEEP){
			//for sucess sleep
			return 0;
		}else{
			/**** select timeout *********/
			TCP_SOCKET_ERR("select timer out!\n");
			return 0;
		}
	}else if(ret == 1){
		return sock;
	}else{
		TCP_SOCKET_ERR("select error , ret = %d\n", ret);
		return -1;
	}
}

/*
return < 0, error 
return = 0, sucess
*/
int clear_sock_receive_buffer(int sock)
{
	int ret;
	char tmp;
	
	if(sock < 0){
		TCP_SOCKET_ERR("sock error , %d\n", sock);
		return -1;
	}

	while(1){
		ret = select_socket(sock, 0, SELECT_READ);
		if(ret > 0){
			recv(sock, &tmp, 1, 0);
		}else if(ret == 0){
			break;
		}else{
			return -1;
		}
	}
	return 0;
}


