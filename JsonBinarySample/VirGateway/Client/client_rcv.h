#ifndef __CLIENT_RCV__H_
#define __CLIENT_RCV__H_

//#define CLIENT_RCV_DBG_ON
#ifdef CLIENT_RCV_DBG_ON
#define CLIENT_RCV_DBG(fmt, args...)	printf("CLIENT RCV DBG[%s][%d]: "fmt, __FILE__, __LINE__, ##args)
#else
#define CLIENT_RCV_DBG(fmt, args...)
#endif

#define CLIENT_RCV_ERR_ON
#ifdef CLIENT_RCV_ERR_ON
#define CLIENT_RCV_ERR(fmt, args...)	printf("CLIENT RCV ERR[%s][%d]: "fmt, __FILE__, __LINE__, ##args)
#else
#define CLIENT_RCV_ERR(fmt, args...)
#endif



void* client_rcv_thread(void* args);
void kill_client_rcv_thread(pthread_t tcp_cloud_rcv_pthread);




#endif //__CLIENT_RCV__H_


