#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

#include "libfunction.h"
#include "client.h"
#include "client_database.h"
#include "client_send.h"

int send_json_data(int sock,int msgid,char *jsonData)
{


	int ret;	
	char data[512] = {0};

	sprintf(data, "{\"t\":3,\"datatype\":1,\"msgid\":%d,\"datas\":{\"jsonsensor\":%s}}", msgid, jsonData);

	printf("/********************************** SEND JSON DATA **********************************/\r\n");
	printf("%s\r\n",data);
	printf("/********************************** ************** **********************************/\r\n\r\n");
	
	ret = send_packet(sock, data, strlen(data), 0);
	if(ret < 0){
		CLIENT_SEND_ERR("send packet to cloud fail\r\n");
		return -1;
	}


	return 0;
}

int send_binary_data(int sock,int msgid,char *binaryData, int binlength)
{

	int ret;	

	char data[512] = {0};
	char base64[256] = {0};

	ret = base64_encode(binaryData, base64, binlength);
	if(ret == BASE64FAIL)
	{
		printf("binary to base64 faill\r\n");
		return -1;
	}
	

	sprintf(data, "{\"t\":3,\"datatype\":1,\"msgid\":%d,\"datas\":{\"binarysensor\":\"%s\"}}", msgid, base64);

	
	printf("/********************************* SEND Binary DATA *********************************/\r\n");
	printf("%s\r\n",data);
	printf("/********************************** ************** **********************************/\r\n\r\n");
	
	ret = send_packet(sock, data, strlen(data), 0);
	if(ret < 0){
		CLIENT_SEND_ERR("send packet to cloud fail\r\n");
		return -1;
	}
	

	return 0;
}




int send_data_to_cloud(void)
{
	char jsonData[] =  "{\"UserName\":\"李四\",\"Age\":23}";
	char binaryData[] = {0x02,0x04,0x06,0x08};
	int binlength = 4;

	int send_sock = tcpSend_fd_read();
	send_json_data(send_sock, 1, jsonData);
	sleep(5);

	send_binary_data(send_sock, 2, binaryData, binlength);
	sleep(5);

	return 0;

}





