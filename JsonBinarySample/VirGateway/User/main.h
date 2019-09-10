#ifndef _MAIN_H_
#define _MAIN_H_


#define MAIN_DBG_ON
#ifdef MAIN_DBG_ON
#define MAIN_DBG(fmt, args...)	printf("MAIN DBG[%s][%d]: "fmt, __FILE__, __LINE__, ##args)
#else
#define MAIN_DBG(fmt, args...)
#endif

#define MAIN_ERR_ON
#ifdef MAIN_ERR_ON
#define MAIN_ERR(fmt, args...)	printf("MAIN ERR[%s][%d]: "fmt, __FILE__, __LINE__, ##args)
#else
#define MAIN_ERR(fmt, args...)
#endif

#define SERIAL_BUF_SIZE 512


//zigbee串口相关宏定义
#define ZIGBEE
#define	ZIGBEE_SERIAL_DEV	"/dev/ttyS1"
#define ZIGBEE_SERIAL_BAUD_RATE	38400

//LORA串口相关宏定义
#define LORA
#define	LORA_SERIAL_DEV	"/dev/ttyUSB3"
#define LORA_SERIAL_BAUD_RATE	115200

//CAN串口相关宏定义
#define CAN
#define	CAN_SERIAL_DEV	"/dev/ttyUSB2"
#define CAN_SERIAL_BAUD_RATE	115200




#endif //_MAIN_H_

