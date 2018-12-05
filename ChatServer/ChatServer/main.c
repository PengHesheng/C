#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <WinSock2.h>
#include <mysql.h>
#pragma comment(lib, "Ws2_32")

void InitMySql(void);
DWORD WINAPI TcpServeThread(VOID * lpParam);
void DoMsgRegister(char * msg, SOCKET sock);
void DoMsgLogin(char * msg, SOCKET sock);
void DoMsgText(char * msg, SOCKET sock);
void DoMsgFile(char * msg, SOCKET sock);

#define MAX_CLIENT 10  //ͬʱ����Ĳ�������������
#define MAX_SUF_SIZE 65535  //���շ��ͻ�����
int TcpClientCount = 0;
#define MYSQL_LOCALHOST "localhost"
#define MYSQL_USER "root"
#define MYSQL_PWD "root"
#define MYSQL_NAME "test"
MYSQL *mysql;
char sql[1024];

//������Ϣͷ
typedef struct ChatHead
{
	UCHAR version;  //�汾�ţ�Ĭ��1.0
	UCHAR length;  //ͷ��
	USHORT size;  //���ݳ���
	/*ע�ᡢ��¼���˺ź����볤�Ⱦ�Ϊ16���ֽ�*/
	UCHAR type;  //Э������ 1->ע�� 2->��½ 3->�ı���Ϣ 4->�ļ���Ϣ 5->ͼƬ��Ϣ
	UCHAR sqe;  //ƫ�ƣ���typeΪ4��Ч��Ĭ��0
	USHORT all_size;  //���ݵ��ܳ��ȣ���sqeʱ��Ч
	DWORD time;  //����ʱ��
} CHATHEAD, *pCHARHEAD;

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
	printf("bind %d\n", WSAGetLastError());
	listen(ListenSocket, SOMAXCONN);  //����
	printf("listen %d\n", WSAGetLastError());

	InitMySql();
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
	mysql_close(mysql);
	return 0;
}

void InitMySql(void)
{
	//��ʼ��MySql
	mysql = NULL;
	mysql = mysql_init((MYSQL*)0);
	if (!mysql) {
		printf("init mysql error\n");
	}
	mysql = mysql_real_connect(mysql, MYSQL_LOCALHOST, MYSQL_USER, MYSQL_PWD, MYSQL_NAME, 0, NULL, 0);
	if (mysql)
	{
		printf("mysql connect success\n");
	}
	else
	{
		printf("mysql connect error:%d, %s\n", mysql_errno(mysql), mysql_error(mysql));
	}
}

DWORD __stdcall TcpServeThread(VOID * lpParam)
{
	pTcpThreadParam pParam = (pTcpThreadParam)lpParam;
	SOCKET TcpSocket = pParam->socket;
	SOCKADDR_IN TcpClientAddr = pParam->addr;
	char ServerTCPBuf[MAX_SUF_SIZE];
	//	send(TcpSocket, ServerTCPBuf, strlen(ServerTCPBuf), 0);  //�����ݸ�ʽ�������������
	while (1)
	{
		memset(ServerTCPBuf, '\0', sizeof(ServerTCPBuf));
		int TCPBytesRcv = recv(TcpSocket, ServerTCPBuf, sizeof(ServerTCPBuf), 0);
		//TCPBytesRcv==0��ʾ�����ر�
		if (TCPBytesRcv == 0 || TCPBytesRcv == SOCKET_ERROR)
		{
			break;
		}
		pCHARHEAD pChatHead = (pCHARHEAD)ServerTCPBuf;
		UCHAR type = pChatHead->type;
		switch (type)
		{
		case 1:
			DoMsgRegister(ServerTCPBuf, TcpSocket);
			break;
		case 2:
			DoMsgLogin(ServerTCPBuf, TcpSocket);
			break;
		case 3:
			DoMsgText(ServerTCPBuf, TcpSocket);
			break;
		case 4:
			DoMsgFile(ServerTCPBuf, TcpSocket);
			break;
		case 5:
			break;
		default:
			break;
		}

	}
	closesocket(TcpSocket);
	InterlockedDecrement(&TcpClientCount);
	return 0;
}

void DoMsgRegister(char * msg, SOCKET sock)
{
	char * data = msg + sizeof(CHATHEAD);
	char * buf = NULL;
	char * name = strtok_s(data, " ", &buf);
	char * pwd = strtok_s(NULL, " ", &buf);
	char * result = (char*)malloc(20);
	memset(result, '\0', sizeof(result));
	memset(sql, '\0', sizeof(sql));
	snprintf(sql, sizeof(sql), "%s%s%s", "select pwd from user where name='", name, "'");
	int flag = mysql_real_query(mysql, sql, (ULONG)strlen(sql));
	if (flag != 0)
	{
		printf("select error:%d, %s\n", mysql_errno(mysql), mysql_error(mysql));
		result = "query failed";
	}
	else
	{
		MYSQL_RES * res = mysql_store_result(mysql);
		MYSQL_ROW row = mysql_fetch_row(res);
		if (res->row_count != 0) {
			sprintf_s(result, 20, "%s", "registered");
		} 
		else
		{
			snprintf(sql, sizeof(sql), "%s%s%s%s%s", "insert into user (name, pwd) values ('", name, "','", pwd, "')");
			if (mysql_real_query(mysql, sql, (ULONG)strlen(sql)))
			{
				printf("insert error:%d, %s\n", mysql_errno(mysql), mysql_error(mysql));
				sprintf_s(result, 20, "%s", "register failed");
			}
			else
			{
				sprintf_s(result, 20, "%s", "register success");
			}
		}
	}
	memset(msg, '\0', MAX_SUF_SIZE);
	pCHARHEAD pChatHead = (pCHARHEAD)msg;
	pChatHead->size = strlen(result);
	pChatHead->type = 3;
	pChatHead->version = 1;
	pChatHead->sqe = 0;
	pChatHead->time = GetTickCount();
	pChatHead->all_size = pChatHead->size;
	pChatHead->length = sizeof(CHATHEAD);
	sprintf_s(msg + sizeof(CHATHEAD), sizeof(CHATHEAD)+strlen(result), "%s", result);
	free(result);
	send(sock, msg, sizeof(CHATHEAD) + strlen(result), 0);
}

void DoMsgLogin(char * msg, SOCKET sock)
{
	char * data = msg + sizeof(CHATHEAD);
	char * buf = NULL;
	char * name = strtok_s(data, " ", &buf);
	char * pwd = strtok_s(NULL, " ", &buf);
	char * result = (char*)malloc(20);
	memset(result, '\0', sizeof(result));
	memset(sql, '\0', sizeof(sql));
	snprintf(sql, sizeof(sql), "%s%s%s", "select pwd from user where name='", name, "'");
	int flag = mysql_real_query(mysql, sql, (ULONG)strlen(sql));
	if (flag != 0)
	{
		printf("select error:%d, %s\n", mysql_errno(mysql), mysql_error(mysql));
		result = "query failed";
	}
	else
	{
		MYSQL_RES * res = mysql_store_result(mysql);
		MYSQL_FIELD * field = mysql_fetch_field(res);
		MYSQL_ROW row = mysql_fetch_row(res);
		if (res->row_count != 0 && strcmp(row[0], pwd) == 0) {
			sprintf_s(result,20,"%s","login success");
		}
		else
		{
			sprintf_s(result,20,"%s","login failed");
		}
	}
	memset(msg, '0', MAX_SUF_SIZE);
	pCHARHEAD pChatHead = (pCHARHEAD)msg;
	pChatHead->size = strlen(result);
	pChatHead->type = 3;
	pChatHead->version = 1;
	pChatHead->sqe = 0;
	pChatHead->time = (ULONG)GetTickCount();
	pChatHead->all_size = pChatHead->size;
	pChatHead->length = (UCHAR)sizeof(CHATHEAD);
	sprintf_s(msg + sizeof(CHATHEAD), sizeof(CHATHEAD) + strlen(result), "%s", result);
	free(result);
	send(sock, msg, sizeof(CHATHEAD) + strlen(result), 0);
}

void DoMsgText(char * msg, SOCKET sock)
{
	char * data = msg + sizeof(CHATHEAD);
	puts("recv: ");
	puts(data);
}

void DoMsgFile(char * msg, SOCKET sock)
{
	pCHARHEAD pChatHead = (pCHARHEAD)msg;
	UCHAR sqe = pChatHead->sqe;
	FILE * file = NULL;
	if (fopen_s(&file, "F:\\C\\source1.txt", "w+"))
	{
		printf("open file error\n");
	}
	int recv_length = pChatHead->size;
	int all_size = pChatHead->all_size;
	int recved_szie = 0;
	while (recv_length > 0)
	{
		fwrite(msg + sizeof(CHATHEAD), recv_length, 1, file);
		recved_szie += recv_length;
		if (recved_szie == all_size) {
			break;
		}
		memset(msg, '\0', MAX_SUF_SIZE);
		recv_length = recv(sock, msg, MAX_SUF_SIZE, 0);
		if (recv_length < 0)
		{
			puts("recv error sqe: %d\n", sqe);
		}
	}
	fclose(file);
	puts("recv file over");
}