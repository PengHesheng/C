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

#define MAX_CLIENT 10  //同时服务的并发连接数上限
#define MAX_SUF_SIZE 65535  //接收发送缓冲区
int TcpClientCount = 0;
#define MYSQL_LOCALHOST "localhost"
#define MYSQL_USER "root"
#define MYSQL_PWD "root"
#define MYSQL_NAME "test"
MYSQL *mysql;
char sql[1024];

//聊天消息头
typedef struct ChatHead
{
	UCHAR version;  //版本号，默认1.0
	UCHAR length;  //头长
	USHORT size;  //数据长度
	/*注册、登录的账号和密码长度均为16个字节*/
	UCHAR type;  //协议类型 1->注册 2->登陆 3->文本消息 4->文件消息 5->图片消息
	UCHAR sqe;  //偏移，当type为4有效，默认0
	USHORT all_size;  //数据的总长度，有sqe时有效
	DWORD time;  //发送时间
} CHATHEAD, *pCHARHEAD;

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
	printf("bind %d\n", WSAGetLastError());
	listen(ListenSocket, SOMAXCONN);  //监听
	printf("listen %d\n", WSAGetLastError());

	InitMySql();
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
	mysql_close(mysql);
	return 0;
}

void InitMySql(void)
{
	//初始化MySql
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
	//	send(TcpSocket, ServerTCPBuf, strlen(ServerTCPBuf), 0);  //将数据格式化输出到缓冲区
	while (1)
	{
		memset(ServerTCPBuf, '\0', sizeof(ServerTCPBuf));
		int TCPBytesRcv = recv(TcpSocket, ServerTCPBuf, sizeof(ServerTCPBuf), 0);
		//TCPBytesRcv==0表示正常关闭
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