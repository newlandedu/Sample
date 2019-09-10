#include <pthread.h>
#include "client_database.h"


/************************ tcpSendFd ************************/
//变量
int tcpSendFd = 0;
pthread_rwlock_t tcpSendFdRWLock;

//函数
void tcpSend_fd_write(int value)
{
	if (pthread_rwlock_wrlock(&tcpSendFdRWLock) != 0){
			CLIENT_BASE_ERR("tcpSendFdRWLock lock fail!\n");
	}
		tcpSendFd = value;
	if (pthread_rwlock_unlock(&tcpSendFdRWLock) != 0){
			CLIENT_BASE_ERR("tcpSendFdRWLock unlock fail!\n");
	}
}

int tcpSend_fd_read(void)
{
	int value;
	if (pthread_rwlock_rdlock(&tcpSendFdRWLock) != 0){
			CLIENT_BASE_ERR("tcpSendFdRWLock lock fail!\n");
	}
		value = tcpSendFd;
	if (pthread_rwlock_unlock(&tcpSendFdRWLock) != 0){
			CLIENT_BASE_ERR("tcpSendFdRWLock unlock fail!\n");
	}

	return value;
}


/************************ ------------- ************************/


/************************ is_network_ok ************************/
int is_network_ok = 0;
pthread_mutex_t is_network_ok_Mutex;

void is_network_ok_write(int value)
{
	if (pthread_mutex_lock(&is_network_ok_Mutex) != 0){
			CLIENT_BASE_ERR("is_network_ok_Mutex lock fail!\n");
	}
		is_network_ok = value;
	if (pthread_mutex_unlock(&is_network_ok_Mutex) != 0){
			CLIENT_BASE_ERR("is_network_ok_Mutex unlock fail!\n");
	}
}

int is_network_ok_read(void)
{
	int value;
	if (pthread_mutex_lock(&is_network_ok_Mutex) != 0){
			CLIENT_BASE_ERR("is_network_ok_Mutex lock fail!\n");
	}
		value = is_network_ok;
	if (pthread_mutex_unlock(&is_network_ok_Mutex) != 0){
			CLIENT_BASE_ERR("is_network_ok_Mutex unlock fail!\n");
	}

	return value;
}

/************************ ------------- ************************/

/************************ is_auth_ok ************************/
int is_auth_ok = 0;
pthread_mutex_t is_auth_ok_Mutex;

void is_auth_ok_write(int value)
{
	if (pthread_mutex_lock(&is_auth_ok_Mutex) != 0){
			CLIENT_BASE_ERR("is_auth_ok_Mutex lock fail!\n");
	}
		is_auth_ok = value;
	if (pthread_mutex_unlock(&is_auth_ok_Mutex) != 0){
			CLIENT_BASE_ERR("is_auth_ok_Mutex unlock fail!\n");
	}

}

int is_auth_ok_read(void)
{
	int value;
	if (pthread_mutex_lock(&is_auth_ok_Mutex) != 0){
			CLIENT_BASE_ERR("is_auth_ok_Mutex lock fail!\n");
	}
		value = is_auth_ok;
	if (pthread_mutex_unlock(&is_auth_ok_Mutex) != 0){
			CLIENT_BASE_ERR("is_auth_ok_Mutex unlock fail!\n");
	}

	return value;
}

/************************ ------------- ************************/

/************************ Heart rate ************************/
int heart_rate_flag = 0;
pthread_mutex_t heart_rate_flag_Mutex;

void heart_rate_flag_write(int value)
{
	if (pthread_mutex_lock(&heart_rate_flag_Mutex) != 0){
			CLIENT_BASE_ERR("heart_rate_flag_Mutex lock fail!\n");
	}
		heart_rate_flag = value;
	if (pthread_mutex_unlock(&heart_rate_flag_Mutex) != 0){
			CLIENT_BASE_ERR("heart_rate_flag_Mutex unlock fail!\n");
	}

}

int heart_rate_flag_read(void)
{
	int value;
	if (pthread_mutex_lock(&heart_rate_flag_Mutex) != 0){
			CLIENT_BASE_ERR("heart_rate_flag_Mutex lock fail!\n");
	}
		value = heart_rate_flag;
	if (pthread_mutex_unlock(&heart_rate_flag_Mutex) != 0){
			CLIENT_BASE_ERR("heart_rate_flag_Mutex unlock fail!\n");
	}

	return value;
}

/************************ ------------- ************************/


/************************ count ************************/
//变量
int count = 0;
pthread_rwlock_t countRWLock;

//函数
void count_write(int value)
{
	if (pthread_rwlock_wrlock(&countRWLock) != 0){
			CLIENT_BASE_ERR("countRWLock lock fail!\n");
	}
		count = value;
	if (pthread_rwlock_unlock(&countRWLock) != 0){
			CLIENT_BASE_ERR("countRWLock unlock fail!\n");
	}

}

int count_read(void)
{
	int value;
	if (pthread_rwlock_rdlock(&countRWLock) != 0){
			CLIENT_BASE_ERR("countRWLock lock fail!\n");
	}
		value = count;
	if (pthread_rwlock_unlock(&countRWLock) != 0){
			CLIENT_BASE_ERR("countRWLock unlock fail!\n");
	}

	return value;
}


/************************ ------------- ************************/



void client_database_lock_init(void)
{
	if(pthread_rwlock_init(&tcpSendFdRWLock, NULL) != 0){
		CLIENT_BASE_ERR("tcpSendFdRWLock init error\r\n"); 
	}

	if (pthread_mutex_init(&is_network_ok_Mutex, NULL) != 0){
		CLIENT_BASE_ERR("is_network_ok_Mutex MutexLock init fail\r\n");
	}

	if (pthread_mutex_init(&is_auth_ok_Mutex, NULL) != 0){
		CLIENT_BASE_ERR("is_network_ok_Mutex MutexLock init fail\r\n");
	}

	if (pthread_mutex_init(&heart_rate_flag_Mutex, NULL) != 0){
		CLIENT_BASE_ERR("heart_rate_flag_Mutex MutexLock init fail\r\n");
	}	

	if(pthread_rwlock_init(&countRWLock, NULL) != 0){
		CLIENT_BASE_ERR("countRWLock init error\r\n"); 
	}

}





