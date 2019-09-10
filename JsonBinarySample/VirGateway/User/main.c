#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>

#include "libfunction.h"
#include "client.h" 
#include "main.h"


int main(void)
{


	MAIN_DBG("Demo start...\r\n");

	//启动TCP客户端线程(用于与云平台通信)
	client_program_start();
	sleep(1);

	pthread_exit(NULL);
	return 0;
}



