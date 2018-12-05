#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <WinSock2.h>
#include <string.h>
#include <time.h>
#pragma comment(lib, "Ws2_32")

DWORD WINAPI TcpServeThread(VOID * lpParam);
DWORD WINAPI UdpServerThread(char * ipaddr);

#define MAX_CLIENT 10  //同时服务的并发连接数上限
#define MAX_SUF_SIZE 65535  //接收发送缓冲区
#define UDP_SRV_PORT 2345  //Server的UDP端口号
int TcpClientCount = 0;
typedef struct TcpThreadParam
{
	SOCKET socket;
	SOCKADDR_IN addr;  //地址信息
}TcpThreadParam, *pTcpThreadParam;

int main(int argc, char * argv[])
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);  //初始化Winsock2环境
	SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  //创建用于倾听TCP的Socket
	char hostname[256];  //本机名
	gethostname(hostname, sizeof(hostname));
	HOSTENT * pHostent = gethostbyname(hostname);  //获取本机IP
	int choose = 0;
	while (pHostent->h_addr_list[choose] != NULL)
	{
		UCHAR *allIp = (UCHAR *)(pHostent->h_addr_list[choose]);
		printf("%d: IP address %d.%d.%d.%d \t%s\n", choose, allIp[0], allIp[1], allIp[2], allIp[3], pHostent->h_name);
		choose++;
	}
	printf("choose IP:");
	scanf_s("%d", &choose);
	//填充本地TCP Listen Socket地址结构
	SOCKADDR_IN ListenAddr;
	ListenAddr.sin_family = AF_INET;
	ListenAddr.sin_port = htons((USHORT)atoi("2346"));
	ListenAddr.sin_addr = *(IN_ADDR*)pHostent->h_addr_list[choose];
	bind(ListenSocket, (SOCKADDR*)&ListenAddr, sizeof(ListenAddr));  //绑定TCP倾听端口
	listen(ListenSocket, SOMAXCONN);  //监听
	printf("listen %d\n", WSAGetLastError());

	//创建UDP服务线程
	DWORD dwUDPThreadId;
	CreateThread(NULL, 0, UdpServerThread, pHostent->h_addr_list[choose], 0, &dwUDPThreadId);
	printf("udp thread id:%d\n", dwUDPThreadId);

	//在一个主循环中接收客户端连接请求并创建服务线程
	SOCKET TcpSocket;
	SOCKADDR_IN TcpClientAddr;
	while (1)
	{
		printf("ready\t");
		//接收客户端连接请求
		int iSockAddrLen = sizeof(SOCKADDR);
		TcpSocket = accept(ListenSocket, (SOCKADDR*)&TcpClientAddr, &iSockAddrLen);
		printf("accept %d\t", WSAGetLastError());
		if (TcpClientCount >= MAX_CLIENT)
		{
			closesocket(TcpSocket);
			continue;
		}
		//设置传递给线程的结构变量参数，创建服务线程
		TcpThreadParam Param;
		Param.socket = TcpSocket;  //与客户端实际连接的Socket
		Param.addr = TcpClientAddr;  //客户端地址结构
		DWORD dwThreadId;
		CreateThread(NULL, 0, TcpServeThread, &Param, 0, &dwThreadId);
		printf("thread id:%d\n", dwThreadId);
		InterlockedIncrement(&TcpClientCount);
	}
	//释放资源
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

	//发送UDP端口号+“START"，由于端口号最大为65535，因此固定使用5个字节
	sprintf_s(ServerTCPBuf, MAX_SUF_SIZE, "%5d%s", UDP_SRV_PORT, "START");
	send(TcpSocket, ServerTCPBuf, strlen(ServerTCPBuf), 0);  //将数据格式化输出到缓冲区
	printf("send port %d\n", WSAGetLastError());
	time_t CurSysTime;
	while (1)
	{
		//读取Client发来的时间请求命令：GET CUR TIME
		memset(ServerTCPBuf, '\0', sizeof(ServerTCPBuf));
		int TCPBytesRcv = recv(TcpSocket, ServerTCPBuf, sizeof(ServerTCPBuf), 0);
		//TCPBytesRcv==0表示正常关闭
		if (TCPBytesRcv == 0 || TCPBytesRcv == SOCKET_ERROR)
		{
			break;
		}

		if (strcmp(ServerTCPBuf, "GET CUR TIME") == 0)
		{
			time(&CurSysTime);
			memset(ServerTCPBuf, '\0', sizeof(ServerTCPBuf));
			/*
			因为localtime并不是线程安全的，观察localtime和localtime_r的调用发现，localtime在使用时，
			我们只需定义一个指针，并不需要为指针申请空间，而指针必须要指向内存空间才可以使用，其实申请空间的动作由函数自己完成，
			这样在多线程的情况下，如果有另一个线程调用了这个函数，那么指针指向的struct tm结构体的数据就会改变。
			在localtime_s与localtime_r调用时，定义的是struct tm的结构体，获取到的时间已经保存在struct tm中，并不会受其他线程的影响
			*/
			struct tm now_time;
			localtime_s(&now_time, &CurSysTime);  //由于localtime不安全，采用localtime_s
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
	//创建UDP Server Socket
	SOCKET UDPServerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//填充本地UDP Socket地址结构
	SOCKADDR_IN UDPSrvAddr;
	memset(&UDPSrvAddr, 0, sizeof(SOCKADDR_IN));
	UDPSrvAddr.sin_family = AF_INET;
	UDPSrvAddr.sin_port = htons(UDP_SRV_PORT);
	UDPSrvAddr.sin_addr = *(IN_ADDR*)ipaddr;
	//UDPSrvAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	//绑定UDP端口
	bind(UDPServerSocket, (SOCKADDR*)&UDPSrvAddr, sizeof(UDPSrvAddr));
	printf("bind %d\n", WSAGetLastError());
	char ServerUDPBuf[MAX_SUF_SIZE];
	SOCKADDR_IN UDPClientAddr;
	while (1)
	{
		memset(ServerUDPBuf, '\0', sizeof(ServerUDPBuf));
		//接收UDP数据
		int iSockAddrLen = sizeof(SOCKADDR);
		recvfrom(UDPServerSocket, ServerUDPBuf, sizeof(ServerUDPBuf), 0, (SOCKADDR*)&UDPClientAddr, &iSockAddrLen);
		printf("from client:%s\n", ServerUDPBuf);
		//回显
		iSockAddrLen = sizeof(SOCKADDR);
		memset(ServerUDPBuf, '\0', sizeof(ServerUDPBuf));
		puts("input");
		gets(ServerUDPBuf);
		sendto(UDPServerSocket, ServerUDPBuf, strlen(ServerUDPBuf), 0, (SOCKADDR*)&UDPClientAddr, iSockAddrLen);
	}
}
