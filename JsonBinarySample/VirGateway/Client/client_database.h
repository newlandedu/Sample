#ifndef _CLIENT_DATABASE_H_
#define _CLIENT_DATABASE_H_

#include<stdio.h>      /*标准输入输出定义*/  
#include<stdlib.h>     /*标准函数库定义*/  
#include<unistd.h>     /*Unix 标准函数定义*/   
#include<string.h>   
#include<stdint.h>


#define CLIENT_BASE_DBG_ON
#ifdef CLIENT_BASE_DBG_ON
#define CLIENT_BASE_DBG(fmt, args...)	printf("CLIENT BASE DBG[%s][%d]: "fmt, __FILE__, __LINE__, ##args)
#else
#define CLIENT_BASE_DBG(fmt, args...)
#endif

#define CLIENT_BASE_ERR_ON
#ifdef CLIENT_BASE_ERR_ON
#define CLIENT_BASE_ERR(fmt, args...)	printf("CLIENT BASE ERR[%s][%d]: "fmt, __FILE__, __LINE__, ##args)
#else
#define CLIENT_BASE_ERR(fmt, args...)
#endif

#define HEART_NONE 0
#define HEART_AT 1
#define HEART_OK 2

void tcpSend_fd_write(int value);
int tcpSend_fd_read(void);

void is_network_ok_write(int value);
int is_network_ok_read(void);

void is_auth_ok_write(int value);
int is_auth_ok_read(void);

void heart_rate_flag_write(int value);
int heart_rate_flag_read(void);

void count_write(int value);
int count_read(void);



void client_database_lock_init(void);












#endif //_CLIENT_DATABASE_H_

