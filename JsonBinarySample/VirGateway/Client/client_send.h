#ifndef _CLIENT_SEND_H_
#define _CLIENT_SEND_H_

//#define CLIENT_SEND_DBG_ON
#ifdef CLIENT_SEND_DBG_ON
#define CLIENT_SEND_DBG(fmt, args...)	printf("CLIENT SEND DBG[%s][%d]: "fmt, __FILE__, __LINE__, ##args)
#else
#define CLIENT_SEND_DBG(fmt, args...)
#endif

#define CLIENT_SEND_ERR_ON
#ifdef CLIENT_SEND_ERR_ON
#define CLIENT_SEND_ERR(fmt, args...)	printf("CLIENT SEND ERR[%s][%d]: "fmt, __FILE__, __LINE__, ##args)
#else
#define CLIENT_SEND_ERR(fmt, args...)
#endif

int send_data_to_cloud(void);
int send_json_data(int sock,int msgid,char *jsonData);
int send_binary_data(int sock,int msgid,char *binaryData, int binlength);



#endif  //_CLIENT_SEND_H_
