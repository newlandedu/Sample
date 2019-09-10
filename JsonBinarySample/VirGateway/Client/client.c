#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>



#include "libfunction.h"
#include "client_rcv.h"
#include "client_send.h"
#include "client_database.h"
#include "client.h"



/*
 *return 1 : 成功
 *		 0 ：失败
 */
int network_check(void)
{	

	if(ping("14.215.177.39") == 0){
		
		is_network_ok_write(1);
		return 1;
	}

	return 0;
}


/*
 *return 2 : 成功
 *		 1 ：失败
 */
int link_server(int *client_sock)
{	

	int ret;
	unsigned short cloudPort = 0;
	char cloudIp[64] = {0};

	//分配sock
	if((*client_sock = open_client_port(NULL, 0)) < 0){
		CLIENT_ERR("open_client_port error\n"); 
		return 1;
	}

	//连接服务器
	if(connection_server(*client_sock, TCP_SERVER_IP, TCP_SERVER_PORT) < 0){
		CLIENT_ERR("connection_server error\n");  
		return 1;
	}else{
		CLIENT_DBG("connection_server success\n"); 
		tcpSend_fd_write(*client_sock);
	}

	return 2;
}

//连接云平台函数
int link_newland_cloud(int sock)
{
	int ret;
	PACKET packet;
	CON_REQ con_req ;

	char deviceId[64] = {0};
	char key[64] = {0};

	if(ret < 0)
	{
		CLIENT_ERR("Not find deviceId and key from 1+x.config\r\n");
		return -1;
	}

	con_req.msg_type = PACKET_TYPE_CONN_REQ ;
	con_req.device_id = GETWAYS_LINK_CLOUD_DEVICE_ID;
	con_req.key   = GETWAYS_LINK_CLOUD_KEY;
	con_req.ver  = GETWAYS_LINK_CLOUD_VAR;
	
	packet = packet_msg(&con_req);

	printf("Link Pack:\r\n%s\r\n",packet);
	
	ret = send_packet(sock, packet, strlen(packet), 0);
	if(ret < 0){
		CLIENT_ERR("PACKET_TYPE_CONN_REQ error\n");
		return -1;
	}
	free_packet_msg(packet);
	return 0;
}


//客户端线程
void* client_thread(void* args)
{
	int choose = 0, heart_count = 0;
	int client_sock;
	pthread_t client_rcv_pthread;
	int server_ans = 0, cloud_ans = 0, ret;

	while(1)
	{

		switch(choose)
		{
			//检查网络
			case 0 : {
				sleep(1);
				choose = network_check();
				break;
			}
			//连接服务器
			case 1 : {
				sleep(1);
				choose = link_server(&client_sock);
				break;
			}
			case 2 : {
				//启动客户端接收线程
				usleep(100 * 1000);
				if(thread_create(&client_rcv_pthread, client_rcv_thread, &client_sock, NULL, NULL, NULL) == -1){
					CLIENT_ERR("create client_rcv_thread error\n");
					sleep(1);
					break;
				}
				choose = 3;
				break;
			}
			//发送链接检测信号 $#AT#
			case 3 : {
				usleep(100 * 1000);
				ret = send_packet(client_sock, KEEP_ALIVE_MSG, strlen(KEEP_ALIVE_MSG), 0);
				if(ret < 0){
					CLIENT_ERR("send KEEP_ALIVE_MSG Error \r\n");
				}else{
					CLIENT_DBG("send KEEP_ALIVE_MSG Success \r\n");
					choose = 4;
					heart_count = 0;
				}
				break;
			}
			//等待服务器响应信号
			case 4 : {
				usleep(300 * 1000);
				if(heart_rate_flag_read() == HEART_OK){
					choose = 5;
					heart_rate_flag_write(HEART_NONE);
				}else{
					server_ans++;
					CLIENT_DBG("Wait server answer %d s\r\n",server_ans);
					if(server_ans > 5){
						CLIENT_ERR("Link server fail,checking network...\r\n");
						kill_client_rcv_thread(client_rcv_pthread);
						choose = 0;
						server_ans = 0;
					}
					break;
				}
				server_ans = 0;
				break;
			}
			//连接云平台
			case 5 : {
				usleep(100 * 1000);
				link_newland_cloud(client_sock);
				tcpSend_fd_write(client_sock);
				choose = 6;
				break;
			}
			//等待连接成功响应
			case 6 : {
				usleep(300 * 1000);
				if(is_auth_ok_read() == 1){
					CLIENT_DBG("Link cloud Success\r\n");
					choose = 7;
				}else{
					cloud_ans++;
					CLIENT_DBG("Wait cloud answer %d s\r\n",cloud_ans);
					if(cloud_ans > 5){
						CLIENT_ERR("Link cloud fail,try to link server\r\n");
						kill_client_rcv_thread(client_rcv_pthread);
						choose = 1;
					}
					break;
				}
				cloud_ans = 0;
				break;
			}
			//发送数据
			case 7 : {
				if(send_data_to_cloud() < 0)
					choose = 5;

				if((is_auth_ok_read() == 0) && (is_network_ok_read() == 0)){
					kill_client_rcv_thread(client_rcv_pthread);
					choose = 0;
				}
				heart_count++;
				if(heart_count == 5) choose = 8;
				else if(heart_count >= 7) choose = 9;
				break;
			}
			//发送心跳请求
			case 8 : {
				ret = send_packet(client_sock, KEEP_ALIVE_MSG, strlen(KEEP_ALIVE_MSG), 0);
				if(ret < 0){
					CLIENT_ERR("send KEEP_ALIVE_MSG Error \r\n");
				}
				choose = 7;
				
				break;
			}
			//判断心跳请求是否有响应
			case 9 : {
				
				if(heart_rate_flag_read() == HEART_OK){
					choose = 7;
					heart_rate_flag_write(HEART_NONE);
					heart_count = 0;
				}else{
	
					CLIENT_ERR("Heart break,checking network...\r\n");
					kill_client_rcv_thread(client_rcv_pthread);
					choose = 0;
					
					break;
				}
				break;
			}

		}
		
	}

}


void client_program_start()
{
	pthread_t client_pthread;

	client_database_lock_init();

	//客户端线程
	if(thread_create(&client_pthread, client_thread, "client_thread", NULL, NULL, NULL) == -1){
		CLIENT_ERR("create client_thread error\n");
	}

}

