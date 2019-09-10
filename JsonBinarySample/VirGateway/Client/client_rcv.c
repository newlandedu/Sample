#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>


#include "libfunction.h"
#include "client.h"
#include "client_database.h"
#include "client_rcv.h"



static void exit_thread(int signo)
{
	CLIENT_RCV_DBG("exit tcp tcv thread\r\n");
	close_connection(tcpSend_fd_read());
	tcpSend_fd_write(0);
	
	pthread_exit(NULL);
}


/*
 *return : 0 : 传感器列表接收成功
 *		   1 ：传感器列表接收中
 *		  -1 ：不是传感器列表
 *		  -2 ：传感器列表接收成功,但解析失败
 */


//心跳数据处理
static int heart_rate_data_deal(int sock, char *msg_buf)
{
	if(strcmp(msg_buf, KEEP_ALIVE_MSG) == 0){
		int ret = send_packet(sock, KEEP_ALIVE_RSP, strlen(KEEP_ALIVE_RSP), 0);
		if(ret < 0){
			CLIENT_RCV_ERR("send keep alive error sock:%d\r\n",sock);
		}else{
			CLIENT_RCV_DBG("send keep alive success sock:%d\r\n",sock);
		}
	}else if(strcmp(msg_buf, KEEP_ALIVE_RSP) == 0){
		heart_rate_flag_write(HEART_OK);
		CLIENT_RCV_DBG("Rcv server heart answer %s\r\n",msg_buf);
	}else{
		CLIENT_RCV_ERR("NOT Json data %s\r\n",msg_buf);
	}

	return 0;
}




static int gataways_send_answer_to_cloud(CMD_REQ_RSP* cmd_rsp )
{
	PACKET packet;
	int ret,sock;
	
	cmd_rsp->msg_type = PACKET_TYPE_CMD_RSP;
	packet = packet_msg(cmd_rsp);
	cmd_rsp->msg_type = PACKET_TYPE_CMD_REQ;
	sock = tcpSend_fd_read();
	if(packet == NULL){
		CLIENT_RCV_ERR("packet_msg PACKET_TYPE_CMD_RSP error\n");
		free_packet_msg(packet);
		return -1;
	}

	CLIENT_RCV_DBG("\r\n%s\r\n",packet);
	
	ret = send_packet(sock, packet, strlen(packet), 0);
	if(ret < 0){
		CLIENT_RCV_ERR("send PACKET_TYPE_CMD_RSP error\n");
		free_packet_msg(packet);
		return -1;
	}else{
		CLIENT_RCV_DBG("cmd rsp:\n%s\n", packet);
	}
	free_packet_msg(packet);
	return 0;
}



static void control_command_deal(void* msg_unpacket)
{
	CMD_REQ* cmd_rcv = (CMD_REQ*)msg_unpacket;
	int is_cmd_need_rsp = 0;
	CMD_REQ_RSP* cmd_rsp = (CMD_REQ_RSP*)cmd_rcv;	//CMD_REQ struct is same with CMD_REQ_RSP struct

	CLIENT_RCV_DBG("recv CMD, data type:%d\n", cmd_rcv->data_type);
	switch(cmd_rcv->data_type){
		case CMD_DATA_TYPE_NUM:
			is_cmd_need_rsp = 1;
		
			CLIENT_RCV_DBG("1.unpacket, msg_type:%d, msg_id:%d apitag:%s, data:%d\n", 
					cmd_rcv->msg_type, cmd_rcv->cmd_id, cmd_rcv->api_tag, *((int*)cmd_rcv->data));
			break;
		case CMD_DATA_TYPE_DOUBLE:
			is_cmd_need_rsp = 1;
			CLIENT_RCV_DBG("2.unpacket, msg_type:%d, msg_id:%d apitag:%s, data:%f\n", 
					cmd_rcv->msg_type, cmd_rcv->cmd_id, cmd_rcv->api_tag, *((double*)cmd_rcv->data));
			break;
		case CMD_DATA_TYPE_STRING:
			is_cmd_need_rsp = 1;

			CLIENT_RCV_DBG("3.unpacket, msg_type:%d, msg_id:%d apitag:%s, data:%s\n", 
					cmd_rcv->msg_type, cmd_rcv->cmd_id, cmd_rcv->api_tag, (char*)cmd_rcv->data);

			cJSON *msg_json = NULL;
			msg_json = cJSON_Parse((char*)cmd_rcv->data);
			if(msg_json != NULL)
			{
					if(msg_json->type == cJSON_Object){
					cJSON *name_json = NULL;
					cJSON *age_json = NULL;
					name_json = cJSON_GetObjectItem(msg_json, "UserName");
					if(name_json && name_json->type != cJSON_String){
						CLIENT_RCV_ERR("name_json ->type(%d) error\n", name_json ->type);
						return NULL;
					}

					age_json = cJSON_GetObjectItem(msg_json, "Age");
					if(age_json && age_json->type != cJSON_Number){
						CLIENT_RCV_ERR("age_json ->type(%d) error\n", age_json ->type);
						return NULL;
					}
					printf("/================ RCV Str DATA ================/\r\n");
					printf("msg_type:%d\r\n",cmd_rcv->msg_type);
					printf("msg_id  :%d\r\n",cmd_rcv->cmd_id);
					printf("apitag  :%s\r\n",cmd_rcv->api_tag);
					printf("UserName:%s\r\n",name_json->valuestring);
					printf("Age     :%d\r\n",age_json->valueint);
					printf("/================ ======== ================/\r\n\r\n");

				}
			}
			else
			{
				printf("/================ RCV Bin DATA ================/\r\n");
				uint8_t bindata[256] = {0};
				int len = base64_decode((char*)cmd_rcv->data, bindata);
				printf("msg_type:%d\r\n",cmd_rcv->msg_type);
				printf("msg_id  :%d\r\n",cmd_rcv->cmd_id);
				printf("apitag  :%s\r\n",cmd_rcv->api_tag);
				printf("apitag  :%s\r\n",cmd_rcv->data);
				printf("changhex:");
				print_hex(bindata,len);
				printf("/================ ======== ================/\r\n\r\n");
			}
				
			
			
			CLIENT_RCV_DBG("3.unpacket, msg_type:%d, msg_id:%d apitag:%s, data:%s\n", 
					cmd_rcv->msg_type, cmd_rcv->cmd_id, cmd_rcv->api_tag, (char*)cmd_rcv->data);
			break;
		case CMD_DATA_TYPE_JSON:
			is_cmd_need_rsp = 1;
			CLIENT_RCV_DBG("4.unpacket, msg_type:%d, msg_id:%d apitag:%s, data:%s\n", 
					cmd_rcv->msg_type, cmd_rcv->cmd_id, cmd_rcv->api_tag, (char*)cmd_rcv->data);
			break;
		default:
			CLIENT_RCV_ERR("data_type(%d) error\n", cmd_rcv->data_type);
	}
	if(is_cmd_need_rsp){
		if(gataways_send_answer_to_cloud(cmd_rsp) == -1){
			CLIENT_RCV_ERR("gataways_send_answer_to_cloud fail");
		}
	}
	
}


static void cloud_answer_data_deal(void* msg_unpacket)
{
	int* msg_type;

	msg_type = (int*)msg_unpacket;
	
	switch(*msg_type)
	{
		case PACKET_TYPE_CONN_RSP:
		{
			CON_REQ_RSP* con_req_rsp = (CON_REQ_RSP*)msg_unpacket;

			CLIENT_RCV_DBG("unpacket, msg_type:%d, status:%d\n", con_req_rsp->msg_type, con_req_rsp->status);
			if(con_req_rsp->status == 0)
			{
				CLIENT_RCV_DBG("server authentication OK\n");
				is_auth_ok_write(1);
				
			}
			break;
		}
		case PACKET_TYPE_POST_RSP:
		{
			POST_REQ_RSP* post_req_rsp = (POST_REQ_RSP*)msg_unpacket;

			CLIENT_RCV_DBG("unpacket, msg_type:%d, msg_id:%d status:%d\n",
						post_req_rsp->msg_type, post_req_rsp->msg_id, post_req_rsp->status);
			if(post_req_rsp->status == 0)
			{
				CLIENT_RCV_DBG("POST SUCESS\n");
			}
			
			break;
		}
		case PACKET_TYPE_CMD_REQ:
		{
			control_command_deal(msg_unpacket);
			break;
		}
		default:
			CLIENT_RCV_ERR("msg_type(%d) error\n", *msg_type);
			break;
	}
					
	
	
}




void* client_rcv_thread(void* args)
{
	char msg_buf[8192];
	void* msg_unpacket;
	int rcv_sock = *((int *)args);
	int ret;

	signal(SIGQUIT, exit_thread);

	printf("client_rcv_thread start\r\n");
	
	while(1)
	{
		memset(msg_buf, 0, sizeof(msg_buf));
		ret = receive_packet(rcv_sock, msg_buf, sizeof(msg_buf), 0);
		if(ret > 0)
		{
			msg_unpacket = unpacket_msg(msg_buf);
				
			if(msg_unpacket == NULL)
			{
				//心跳请求
				heart_rate_data_deal(rcv_sock, msg_buf);
			}
			else
			{
				//数据
				count_write(0);
				cloud_answer_data_deal(msg_unpacket);
			}
			free_unpacket_msg(msg_unpacket);
			
		}
		else
		{
			break;
		}
	}

	CLIENT_RCV_ERR("Client Rcv Thread exit (sock:%d)\r\n",rcv_sock);
	close_connection(rcv_sock);
	is_auth_ok_write(0);
	is_network_ok_write(0);
	tcpSend_fd_write(0);
	return 0;
	
}

void kill_client_rcv_thread(pthread_t tcp_cloud_rcv_pthread)
{
	CLIENT_RCV_DBG("Killing client rcv thread\r\n");
	pthread_kill(tcp_cloud_rcv_pthread,SIGQUIT);

	CLIENT_RCV_DBG("Waiting client rcv thread exit...\r\n");
	pthread_join(tcp_cloud_rcv_pthread,NULL);

	CLIENT_RCV_DBG("client rcv thread exited\r\n");
}



