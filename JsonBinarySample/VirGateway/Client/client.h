#ifndef _CLIENT_H_
#define _CLIENT_H_

#define CLIENT_DBG_ON
#ifdef CLIENT_DBG_ON
#define CLIENT_DBG(fmt, args...)	printf("CLIENT DBG[%s][%d]: "fmt, __FILE__, __LINE__, ##args)
#else
#define CLIENT_DBG(fmt, args...)
#endif

#define CLIENT_ERR_ON
#ifdef CLIENT_ERR_ON
#define CLIENT_ERR(fmt, args...)	printf("CLIENT ERR[%s][%d]: "fmt, __FILE__, __LINE__, ##args)
#else
#define CLIENT_ERR(fmt, args...)
#endif

#define	KEEP_ALIVE_MSG	"$#AT#\r"
#define	KEEP_ALIVE_RSP	"$OK##\r"

//新大陆正式版云平台服务器地址
//#define TCP_SERVER_IP "120.77.58.34"
//#define TCP_SERVER_PORT	8800

//新大陆测试版云平台服务器地址
#define TCP_SERVER_IP "192.168.14.251"
#define TCP_SERVER_PORT	8600


//设备标识和传输密钥
#define GETWAYS_LINK_CLOUD_DEVICE_ID "virgateway002"
#define GETWAYS_LINK_CLOUD_KEY "70e5f478807e4e2bbca85959c0753a2e"
#define GETWAYS_LINK_CLOUD_VAR "V3.1"

//二进制执行器标识名
#define BINARY_ACTUATOR "binaryactuator"




void client_program_start();


#endif //_CLIENT_H_

