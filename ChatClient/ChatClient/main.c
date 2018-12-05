#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32")

void Login(SOCKET sock, char * buf);
void Register(SOCKET sock, char * buf);
void SendMsg(SOCKET sock, char * buf);
void SendTextMsg(SOCKET sock, char * buf);
void SendFileMsg(SOCKET sock, char * buf);
void TCPConnect(SOCKET tcpSock, char * clientBuf);

#define MAX_SUF_SIZE 65535  //���շ��ͻ�����

//������Ϣͷ
typedef struct ChatHead
{
	UCHAR version;  //�汾�ţ�Ĭ��1.0
	UCHAR length;  //ͷ��
	USHORT size;  //���ݳ���
	/*ע�ᡢ��¼���˺ź����볤�����Ϊ16���ֽڣ��Կո������*/
	UCHAR type;  //Э������ 1->ע�� 2->��½ 3->�ı���Ϣ 4->�ļ���Ϣ 5->ͼƬ��Ϣ
	UCHAR sqe;  //ƫ�ƣ���typeΪ4��Ч��Ĭ��0
	USHORT all_size;  //���ݵ��ܳ��ȣ���sqeʱ��Ч
	DWORD time;  //����ʱ��
} CHATHEAD, *pCHARHEAD;

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);  //��ʼ��Winsock2����
	SOCKET TCPSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  //��������TCP��Socket
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
	TCPServer.sin_family = AF_INET;
	TCPServer.sin_port = htons((USHORT)atoi("2346"));
	TCPServer.sin_addr = *(IN_ADDR*)pHostent->h_addr_list[choose];
	connect(TCPSocket, (SOCKADDR*)&TCPServer, sizeof(TCPServer));
	printf("connect %d\n", WSAGetLastError());

	//�������Ӻ��ȡUDP�˿ںź�����������"START"
	char ClientBuf[MAX_SUF_SIZE];
	memset(ClientBuf, '\0', sizeof(ClientBuf));

	int choice;
	puts("1 register");
	puts("2 login");
	scanf_s("%d", &choice);
	switch (choice)
	{
	case 1:
		//getchar();
		Register(TCPSocket, ClientBuf);
		break;
	case 2:
		//getchar();
		Login(TCPSocket, ClientBuf);
		break;
	default:
		break;
	}
	getchar();
	return 0;
}

void Register(SOCKET sock, char * buf)
{
	char * name = (char*)malloc(16);
	char * pwd = (char*)malloc(16);
	memset(name, '\0', sizeof(name));
	memset(pwd, '\0', sizeof(pwd));
	printf("input your name:");
	scanf_s("%s", name, 16);
	printf("input your pwd:");
	scanf_s("%s", pwd, 16);
	memset(buf, '\0', MAX_SUF_SIZE);
	pCHARHEAD pChatHead = (pCHARHEAD)buf;
	pChatHead->size = strlen(name) + strlen(pwd) + 1;
	pChatHead->all_size = pChatHead->size;
	pChatHead->type = 1;
	pChatHead->version = 1;
	pChatHead->sqe = 0;
	pChatHead->time = GetTickCount();
	pChatHead->length = sizeof(CHATHEAD);
	sprintf_s(buf + sizeof(CHATHEAD), pChatHead->size + pChatHead->length, "%s %s", name, pwd);
	send(sock, buf, pChatHead->size + pChatHead->length, 0);
	free(name);
	free(pwd);
	memset(buf, '\0', MAX_SUF_SIZE);
	int recvTag = recv(sock, buf, MAX_SUF_SIZE, 0);
	if (recvTag == 0 || recvTag == SOCKET_ERROR)
	{
		return;
	}
	char * result = buf + sizeof(CHATHEAD);
	puts(result);
	if (strcmp(result, "register success") == 0)
	{
		
		Login(sock, buf);
	} 
	else
	{
		return;
	}
}

void Login(SOCKET sock, char * buf)
{
	puts("Login:");
	char * name = (char*)malloc(16);
	char * pwd = (char*)malloc(16);
	memset(name, '\0', sizeof(name));
	memset(pwd, '\0', sizeof(pwd));
	printf("input your name:");
	scanf_s("%s", name, 16);
	printf("input your pwd:");
	scanf_s("%s", pwd, 16);
	memset(buf, '0', MAX_SUF_SIZE);
	pCHARHEAD pChatHead = (pCHARHEAD)buf; 
	pChatHead->size = strlen(name) + strlen(pwd) + 1;
	pChatHead->all_size = pChatHead->size;
	pChatHead->type = 2;
	pChatHead->version = 1;
	pChatHead->sqe = 0;
	pChatHead->time = GetTickCount();
	pChatHead->length = sizeof(CHATHEAD);
	sprintf_s(buf + sizeof(CHATHEAD), pChatHead->size + pChatHead->length, "%s %s", name, pwd);
	//��������'\0'�ַ������Բ��ܸ���strlen�������������ֻ�Ƚϴ�
	send(sock, buf, pChatHead->size + pChatHead->length, 0);
	free(name);
	free(pwd);
	memset(buf, '0', MAX_SUF_SIZE);
	int recvTag = recv(sock, buf, MAX_SUF_SIZE, 0);
	if (recvTag == 0 || recvTag == SOCKET_ERROR)
	{
		return;
	}
	char * result = buf + sizeof(CHATHEAD);
	puts(result);
	if (strcmp(result, "login success") == 0)
	{
		SendMsg(sock, buf);
	}
	else
	{
		return;
	}
}

void SendMsg(SOCKET sock, char * buf)
{
	printf("intput your choice:\n");
	int choice;
	while (1)
	{
		puts("1 send text msg");
		puts("2 send file");
		puts("3 close connect");
		scanf_s("%d", &choice);
		switch (choice)
		{
		case 1:
			getchar();
			SendTextMsg(sock, buf);
			break;
		case 2:
			getchar();
			SendFileMsg(sock, buf);
			break;
		case 3:
			WSACleanup();
			printf("close\n");
			return;
		default:
			break;
		}
	}
}

void SendTextMsg(SOCKET sock, char * buf)
{
	printf("inpput content:");
	memset(buf, '\0', MAX_SUF_SIZE);
	pCHARHEAD pChatHead = (pCHARHEAD)buf;
	pChatHead->type = 3;
	pChatHead->version = 1;
	pChatHead->sqe = 0;
	pChatHead->time = GetTickCount();
	pChatHead->length = sizeof(CHATHEAD);
	gets(buf + sizeof(CHATHEAD));
	char * msg = buf + sizeof(CHATHEAD);
	pChatHead->size = strlen(msg); 
	send(sock, buf, pChatHead->size + pChatHead->length, 0);
}

void SendFileMsg(SOCKET sock, char * buf)
{
	pCHARHEAD pChatHead = (pCHARHEAD)buf;
	int sqe = 0;
	FILE * file = NULL;
	if (fopen_s(&file, "F:\\C\\source.txt", "r"))
	{
		printf("open file error\n");
	}
	HANDLE handle = CreateFile("F:\\C\\source.txt", FILE_READ_EA, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	int file_size = GetFileSize(handle, NULL);
	for (sqe = 0; !feof(file); sqe++)
	{
		memset(buf, '\0', MAX_SUF_SIZE);
		char * data = buf + sizeof(CHATHEAD);
		fread(data, 1024, 1, file);
		pChatHead->type = 4;
		pChatHead->version = 1;
		pChatHead->sqe = sqe;
		pChatHead->time = GetTickCount();
		pChatHead->length = sizeof(CHATHEAD);
		pChatHead->size = 1024 > file_size ? file_size : 1024;
		pChatHead->all_size = file_size;
		send(sock, buf, pChatHead->size + pChatHead->length, 0);
	}
	fclose(file);
	puts("send file over");
}

void TCPConnect(SOCKET tcpSock, char * clientBuf)
{
	memset(clientBuf, '\0', MAX_SUF_SIZE);  //����ָ���ԭ�򣬲���ͨ��sizeofȥָ����������С������ղ���Ҳ�����
	getchar();
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

