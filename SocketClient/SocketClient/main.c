#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32")

void TCPConnect(SOCKET tcpSock, char * clientBuf);
void UDPConnect(SOCKET udpSock, char * clientBuf, SOCKADDR_IN * udpSrvAddr);

#define MAX_SUF_SIZE 65535  //���շ��ͻ�����

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);  //��ʼ��Winsock2����
	SOCKET TCPSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  //��������TCP��Socket
	SOCKET UDPSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
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

	SOCKADDR_IN TCPServer;
	//memset(&TCPServer, 0, sizeof(SOCKADDR_IN));
	TCPServer.sin_family = AF_INET;
	TCPServer.sin_port = htons((USHORT)atoi("2346"));
	//TCPServer.sin_addr.S_un.S_addr = inet_addr(argv[1]);
	TCPServer.sin_addr = *(IN_ADDR*)pHostent->h_addr_list[choose];
	connect(TCPSocket, (SOCKADDR*)&TCPServer, sizeof(TCPServer));
	printf("connect %d\n", WSAGetLastError());

	//�������Ӻ��ȡUDP�˿ںź�����������"START"
	char ClientBuf[MAX_SUF_SIZE];
	memset(ClientBuf, '\0', sizeof(ClientBuf));
	char portnum[5];  //����UDP�˿ںŻ�����
	int BytesRecv = recv(TCPSocket, ClientBuf, sizeof(ClientBuf), 0);
	memcpy(portnum, ClientBuf, 5);  //�˿ں��ַ����̶�5���ֽ�
	USHORT ServerUDPPort = (USHORT)atoi(portnum);
	if (strcmp("START", ClientBuf + 5) != 0)
	{
		printf("error\n");
		getchar();
		return -1;
	}

	//��䱾��UDP Socket��ַ�ṹ
	SOCKADDR_IN UDPClientAddr;
	memset(&UDPClientAddr, 0, sizeof(SOCKADDR_IN));
	UDPClientAddr.sin_family = AF_INET;
	UDPClientAddr.sin_port = htons(ServerUDPPort);
	UDPClientAddr.sin_addr = *(IN_ADDR*)pHostent->h_addr_list[choose];
	//��UDP�˿�
	//bind(UDPSocket, (SOCKADDR*)&UDPClientAddr, sizeof(UDPClientAddr));
	//printf("bind %d\n", WSAGetLastError());
	//SOCKADDR_IN UDPServeAddr;

	int choice;
	while (1)
	{
		printf("1 is TCP to get system time\n2 is UDP\n3 is close\n");
		scanf_s("%d", &choice);
		switch (choice)
		{
		case 1:
			TCPConnect(TCPSocket, ClientBuf);
			break;
		case 2:
			UDPConnect(UDPSocket, ClientBuf, &UDPClientAddr);
			break;
		case 3:
			closesocket(TCPSocket);
			closesocket(UDPSocket);
			WSACleanup();
			printf("close\n");
			getchar();
			return 0;
		default:
			break;
		}
	}
	return 0;
}

void TCPConnect(SOCKET tcpSock, char * clientBuf)
{
	memset(clientBuf, '\0', MAX_SUF_SIZE);  //����ָ���ԭ�򣬲���ͨ��sizeofȥָ����������С������ղ���Ҳ�����
	char c[1];
	gets(c);
	printf("input GET CUR TIME to get system time:");
	gets(clientBuf);
	puts(clientBuf);
	//sprintf_s(clientBuf, MAX_SUF_SIZE, "%s", "GET CUR TIME");
	send(tcpSock, clientBuf, strlen(clientBuf), 0);
	memset(clientBuf, '\0', MAX_SUF_SIZE);
	int TCPBytesRcv = recv(tcpSock, clientBuf, MAX_SUF_SIZE, 0);
	//TCPBytesRcv==0��ʾ�����ر�
	if (TCPBytesRcv == 0 || TCPBytesRcv == SOCKET_ERROR)
	{
		printf("close or socket error\n");
		return;
	}
	printf("cur time is %s\n", clientBuf);
}

void UDPConnect(SOCKET udpSock, char * clientBuf, SOCKADDR_IN * udpSrvAddr)
{
	char c[1];
	gets(c);
	memset(clientBuf, '\0', MAX_SUF_SIZE);
	int iSockAddrLen = sizeof(SOCKADDR);
	printf("input content��");
	gets(clientBuf);
	//scanf_s("%s", clientBuf, MAX_SUF_SIZE);
	sendto(udpSock, clientBuf, strlen(clientBuf), 0, (SOCKADDR*)udpSrvAddr, iSockAddrLen);

	memset(clientBuf, '\0', MAX_SUF_SIZE);
	iSockAddrLen = sizeof(SOCKADDR);
	recvfrom(udpSock, clientBuf, MAX_SUF_SIZE, 0, (SOCKADDR*)udpSrvAddr, &iSockAddrLen);  //����ָ���ԭ�򣬲���ͨ��sizeofȥָ����������С������ղ���Ҳ�����
	printf("from server:%s\n", clientBuf);
}