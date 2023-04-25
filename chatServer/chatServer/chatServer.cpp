#include <stdio.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)
#include <string.h>

int main()
{
	//1. �����׽��ֿ�
	WSADATA wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0)
	{
		printf("�����׽��ֿ�ʧ��.������룺%d\n", GetLastError());
		return 0;
	}

	//2.�ж�����İ汾�Ƿ�һ��
	if (LOBYTE(wd.wVersion) != 2 || HIBYTE(wd.wVersion) != 2)
	{
		printf("������׽��ְ汾��һ��.������룺%d\n", GetLastError());
		return 0;
	}

	//3.�����׽���
	int sockSrv = socket(AF_INET, SOCK_STREAM, 0);
	if (sockSrv < 0)
	{
		printf("�����׽���ʧ��.�������:%d\n", GetLastError());
		return 0;
	}

	//4.��
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8080);
	int f = bind(sockSrv, (sockaddr*)&addrSrv, sizeof(addrSrv));
	if (f < 0)
	{
		printf("���׽���ʧ��.�������:%d\n", GetLastError());
		return 0;
	}

	//5.����
	int l = listen(sockSrv, 10);
	if (l < 0)
	{
		printf("����ʧ��.�������:%d\n", GetLastError());
		return 0;
	}

	//6.����
	printf("�ȴ��ͻ������ӣ�����\n");
	SOCKADDR_IN addrClient; 
	int length = sizeof(SOCKADDR);
	while (1)
	{
		SOCKET sockFd = accept(sockSrv, (SOCKADDR *)&addrClient, &length);
		if (sockFd == SOCKET_ERROR)
		{
			printf("����ʧ��.�������:%d\n", GetLastError());
			return 0;
		}
		printf("��ӭ�ͻ���:%s:%d���ӷ�����\n", inet_ntoa(addrClient.sin_addr), addrClient.sin_port);

		//������Ϣ���ͻ���
		
		while (1)
		{
			char sendMsg[1000] = {0};
			printf("��:");
			scanf("%s", sendMsg);
			if (strcmp(sendMsg, "q") == 0)
			{
				printf("��������\n");
				closesocket(sockFd);
				break;
			}
			//��������
			send(sockFd, sendMsg, strlen(sendMsg), 0);

			//��������
			char rcvMsg[1024] = {0};
			memset(rcvMsg, 0, sizeof(rcvMsg));
			recv(sockFd, rcvMsg, sizeof(rcvMsg), 0);
			printf("�Է�:%s\r\n", rcvMsg);
		}
	}
	return 0;
}