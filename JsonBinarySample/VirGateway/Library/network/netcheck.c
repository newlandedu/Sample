#include "netcheck.h"



int ping(char *ip)
{
	int i, status;
   	pid_t pid;
 
	// 不同则循环检测多次
    for (i = 0; i < 1; ++i)
    {
		// 新建一个进程来执行ping命令
        if ((pid = vfork()) < 0) 
        {
        	printf("vfork error");
            continue;
        } 
       
		if (pid == 0) 
        {
			// 执行ping命令
        	if (execlp("ping", "ping", "-c", "1", "-q", ip, (char*)0) < 0)
            {
            	printf("execlp error\n");
                exit(1);
            }
        }
		
       	waitpid(pid, &status, 0);
 
		// 相等说明正常
       	if (status == 0)
       		return 0;
 
        sleep(2);
	}
 
    return -1;
}

int reboot()
{
	int i, status;
   	pid_t pid;
 
	// 不同则循环检测多次
    for (i = 0; i < 1; ++i)
    {
		// 新建一个进程来执行ping命令
        if ((pid = vfork()) < 0) 
        {
        	printf("vfork error");
            continue;
        } 
       
		if (pid == 0) 
        {
			// 执行ping命令
        	if (execlp("reboot", "reboot", (char*)0) < 0)
            {
            	printf("execlp error\n");
                exit(1);
            }
        }
		
       	waitpid(pid, &status, 0);
 
		// 相等说明正常
       	if (status == 0)
       		return 0;
 
        sleep(2);
	}
 
    return -1;
}


 




