#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <WinSock2.h>
#include <string.h>
#include <time.h>
#pragma comment(lib, "Ws2_32")

DWORD WINAPI TcpServeThread(VOID * lpParam);
DWORD WINAPI UdpServerThread(char * ipaddr);

#define MAX_CLIENT 10  //ͬʱ����Ĳ�������������
#define MAX_SUF_SIZE 65535  //���շ��ͻ�����
#define UDP_SRV_PORT 2345  //Server��UDP�˿ں�
int TcpClientCount = 0;
typedef struct TcpThreadParam
{
	SOCKET socket;
	SOCKADDR_IN addr;  //��ַ��Ϣ
}TcpThreadParam, *pTcpThreadParam;

int main(int argc, char * argv[])
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);  //��ʼ��Winsock2����
	SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  //������������TCP��Socket
	char hostname[256];  //������
	gethostname(hostname, sizeof(hostname));
	HOSTENT * pHostent = gethostbyname(hostname);  //��ȡ����IP
	int choose = 0;
	while (pHostent->h_addr_list[choose] != NULL)
	{
		UCHAR *allIp = (UCHAR *)(pHostent->h_addr_list[choose]);
		printf("%d: IP address %d.%d.%d.%d \t%s\n", choose, allIp[0], allIp[1], allIp[2], allIp[3], pHostent->h_name);
		choose++;
	}
	printf("choose IP:");
	scanf_s("%d", &choose);
	//��䱾��TCP Listen Socket��ַ�ṹ
	SOCKADDR_IN ListenAddr;
	ListenAddr.sin_family = AF_INET;
	ListenAddr.sin_port = htons((USHORT)atoi("2346"));
	ListenAddr.sin_addr = *(IN_ADDR*)pHostent->h_addr_list[choose];
	bind(ListenSocket, (SOCKADDR*)&ListenAddr, sizeof(ListenAddr));  //��TCP�����˿�
	listen(ListenSocket, SOMAXCONN);  //����
	printf("listen %d\n", WSAGetLastError());

	//����UDP�����߳�
	DWORD dwUDPThreadId;
	CreateThread(NULL, 0, UdpServerThread, pHostent->h_addr_list[choose], 0, &dwUDPThreadId);
	printf("udp thread id:%d\n", dwUDPThreadId);

	//��һ����ѭ���н��տͻ����������󲢴��������߳�
	SOCKET TcpSocket;
	SOCKADDR_IN TcpClientAddr;
	while (1)
	{
		printf("ready\t");
		//���տͻ�����������
		int iSockAddrLen = sizeof(SOCKADDR);
		TcpSocket = accept(ListenSocket, (SOCKADDR*)&TcpClientAddr, &iSockAddrLen);
		printf("accept %d\t", WSAGetLastError());
		if (TcpClientCount >= MAX_CLIENT)
		{
			closesocket(TcpSocket);
			continue;
		}
		//���ô��ݸ��̵߳Ľṹ�������������������߳�
		TcpThreadParam Param;
		Param.socket = TcpSocket;  //��ͻ���ʵ�����ӵ�Socket
		Param.addr = TcpClientAddr;  //�ͻ��˵�ַ�ṹ
		DWORD dwThreadId;
		CreateThread(NULL, 0, TcpServeThread, &Param, 0, &dwThreadId);
		printf("thread id:%d\n", dwThreadId);
		InterlockedIncrement(&TcpClientCount);
	}
	//�ͷ���Դ
	closesocket(ListenSocket);
	WSACleanup();
	return 0;
}

DWORD __stdcall TcpServeThread(VOID * lpParam)
{
	pTcpThreadParam pParam = (pTcpThreadParam)lpParam;
	SOCKET TcpSocket = pParam->socket;
	SOCKADDR_IN TcpClientAddr = pParam->addr;
	char ServerTCPBuf[MAX_SUF_SIZE];

	//����UDP�˿ں�+��START"�����ڶ˿ں����Ϊ65535����˹̶�ʹ��5���ֽ�
	sprintf_s(ServerTCPBuf, MAX_SUF_SIZE, "%5d%s", UDP_SRV_PORT, "START");
	send(TcpSocket, ServerTCPBuf, strlen(ServerTCPBuf), 0);  //�����ݸ�ʽ�������������
	printf("send port %d\n", WSAGetLastError());
	time_t CurSysTime;
	while (1)
	{
		//��ȡClient������ʱ���������GET CUR TIME
		memset(ServerTCPBuf, '\0', sizeof(ServerTCPBuf));
		int TCPBytesRcv = recv(TcpSocket, ServerTCPBuf, sizeof(ServerTCPBuf), 0);
		//TCPBytesRcv==0��ʾ�����ر�
		if (TCPBytesRcv == 0 || TCPBytesRcv == SOCKET_ERROR)
		{
			break;
		}

		if (strcmp(ServerTCPBuf, "GET CUR TIME") == 0)
		{
			time(&CurSysTime);
			memset(ServerTCPBuf, '\0', sizeof(ServerTCPBuf));
			/*
			��Ϊlocaltime�������̰߳�ȫ�ģ��۲�localtime��localtime_r�ĵ��÷��֣�localtime��ʹ��ʱ��
			����ֻ�趨��һ��ָ�룬������ҪΪָ������ռ䣬��ָ�����Ҫָ���ڴ�ռ�ſ���ʹ�ã���ʵ����ռ�Ķ����ɺ����Լ���ɣ�
			�����ڶ��̵߳�����£��������һ���̵߳����������������ôָ��ָ���struct tm�ṹ������ݾͻ�ı䡣
			��localtime_s��localtime_r����ʱ���������struct tm�Ľṹ�壬��ȡ����ʱ���Ѿ�������struct tm�У��������������̵߳�Ӱ��
			*/
			struct tm now_time;
			localtime_s(&now_time, &CurSysTime);  //����localtime����ȫ������localtime_s
			strftime(ServerTCPBuf, sizeof(ServerTCPBuf), "%Y-%m-%d %H:%M:%S", &now_time);
			send(TcpSocket, ServerTCPBuf, strlen(ServerTCPBuf), 0);
		}
	}
	closesocket(TcpSocket);
	InterlockedDecrement(&TcpClientCount);
	return 0;
}

DWORD WINAPI UdpServerThread(char * ipaddr)
{
	//����UDP Server Socket
	SOCKET UDPServerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//��䱾��UDP Socket��ַ�ṹ
	SOCKADDR_IN UDPSrvAddr;
	memset(&UDPSrvAddr, 0, sizeof(SOCKADDR_IN));
	UDPSrvAddr.sin_family = AF_INET;
	UDPSrvAddr.sin_port = htons(UDP_SRV_PORT);
	UDPSrvAddr.sin_addr = *(IN_ADDR*)ipaddr;
	//UDPSrvAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	//��UDP�˿�
	bind(UDPServerSocket, (SOCKADDR*)&UDPSrvAddr, sizeof(UDPSrvAddr));
	printf("bind %d\n", WSAGetLastError());
	char ServerUDPBuf[MAX_SUF_SIZE];
	SOCKADDR_IN UDPClientAddr;
	while (1)
	{
		memset(ServerUDPBuf, '\0', sizeof(ServerUDPBuf));
		//����UDP����
		int iSockAddrLen = sizeof(SOCKADDR);
		recvfrom(UDPServerSocket, ServerUDPBuf, sizeof(ServerUDPBuf), 0, (SOCKADDR*)&UDPClientAddr, &iSockAddrLen);
		printf("from client:%s\n", ServerUDPBuf);
		//����
		iSockAddrLen = sizeof(SOCKADDR);
		memset(ServerUDPBuf, '\0', sizeof(ServerUDPBuf));
		puts("input");
		gets(ServerUDPBuf);
		sendto(UDPServerSocket, ServerUDPBuf, strlen(ServerUDPBuf), 0, (SOCKADDR*)&UDPClientAddr, iSockAddrLen);
	}
}
