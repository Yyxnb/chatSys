#include <stdio.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)
#include <string.h>

int main()
{
	//1. 加载套接字库
	WSADATA wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0)
	{
		printf("加载套接字库失败.错误代码：%d\n", GetLastError());
		return 0;
	}

	//2.判断请求的版本是否一致
	if (LOBYTE(wd.wVersion) != 2 || HIBYTE(wd.wVersion) != 2)
	{
		printf("请求的套接字版本不一致.错误代码：%d\n", GetLastError());
		return 0;
	}

	//3.创建套接字
	int sockSrv = socket(AF_INET, SOCK_STREAM, 0);
	if (sockSrv < 0)
	{
		printf("创建套接字失败.错误代码:%d\n", GetLastError());
		return 0;
	}

	//4.绑定
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8080);
	int f = bind(sockSrv, (sockaddr*)&addrSrv, sizeof(addrSrv));
	if (f < 0)
	{
		printf("绑定套接字失败.错误代码:%d\n", GetLastError());
		return 0;
	}

	//5.监听
	int l = listen(sockSrv, 10);
	if (l < 0)
	{
		printf("监听失败.错误代码:%d\n", GetLastError());
		return 0;
	}

	//6.接收
	printf("等待客户端连接！！！\n");
	SOCKADDR_IN addrClient; 
	int length = sizeof(SOCKADDR);
	while (1)
	{
		SOCKET sockFd = accept(sockSrv, (SOCKADDR *)&addrClient, &length);
		if (sockFd == SOCKET_ERROR)
		{
			printf("接收失败.错误代码:%d\n", GetLastError());
			return 0;
		}
		printf("欢迎客户端:%s:%d连接服务器\n", inet_ntoa(addrClient.sin_addr), addrClient.sin_port);

		//发送信息给客户端
		
		while (1)
		{
			char sendMsg[1000] = {0};
			printf("我:");
			scanf("%s", sendMsg);
			if (strcmp(sendMsg, "q") == 0)
			{
				printf("结束聊天\n");
				closesocket(sockFd);
				break;
			}
			//发送数据
			send(sockFd, sendMsg, strlen(sendMsg), 0);

			//接收数据
			char rcvMsg[1024] = {0};
			memset(rcvMsg, 0, sizeof(rcvMsg));
			recv(sockFd, rcvMsg, sizeof(rcvMsg), 0);
			printf("对方:%s\r\n", rcvMsg);
		}
	}
	return 0;
}