#include <stdio.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

int main()
{
	char ipAddress[32];
	printf("����������Ҫ���ӵķ�����ip��ַ:");
	scanf("%s", ipAddress);
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
	int sockClient = socket(AF_INET, SOCK_STREAM, 0);
	if (sockClient < 0)
	{
		printf("�����׽���ʧ��.�������:%d\n", GetLastError());
		return 0;
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(ipAddress);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8080);
	//���ӷ�����
	if (connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) < 0)
	{
		printf("���ӷ�����ʧ��.�������:%d\n", GetLastError());
		return 0;
	}
	
	//������Ϣ����������
	

	while (1)
	{
		//��������
		char rcvMsg[1024] = { 0 };
		memset(rcvMsg, 0, sizeof(rcvMsg));
		recv(sockClient, rcvMsg, sizeof(rcvMsg), 0);
		printf("�Է�:%s\r\n", rcvMsg);

		//��������
		char sendMsg[1024] = {0};
		printf("��:");
		scanf("%s", sendMsg);
		if (strcmp(sendMsg, "q") == 0)
		{
			printf("��������\n");
			closesocket(sockClient);
			break;
		}

		//��������
		send(sockClient, sendMsg, strlen(sendMsg), 0);	
	}

	return 0;
}