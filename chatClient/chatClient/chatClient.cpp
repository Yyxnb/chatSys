#include <stdio.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

int main()
{
	char ipAddress[32];
	printf("请输入你需要连接的服务器ip地址:");
	scanf("%s", ipAddress);
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
	int sockClient = socket(AF_INET, SOCK_STREAM, 0);
	if (sockClient < 0)
	{
		printf("创建套接字失败.错误代码:%d\n", GetLastError());
		return 0;
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(ipAddress);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8080);
	//连接服务器
	if (connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) < 0)
	{
		printf("连接服务器失败.错误代码:%d\n", GetLastError());
		return 0;
	}
	
	//发送信息给服务器端
	

	while (1)
	{
		//接收数据
		char rcvMsg[1024] = { 0 };
		memset(rcvMsg, 0, sizeof(rcvMsg));
		recv(sockClient, rcvMsg, sizeof(rcvMsg), 0);
		printf("对方:%s\r\n", rcvMsg);

		//发送数据
		char sendMsg[1024] = {0};
		printf("我:");
		scanf("%s", sendMsg);
		if (strcmp(sendMsg, "q") == 0)
		{
			printf("结束聊天\n");
			closesocket(sockClient);
			break;
		}

		//发送数据
		send(sockClient, sendMsg, strlen(sendMsg), 0);	
	}

	return 0;
}