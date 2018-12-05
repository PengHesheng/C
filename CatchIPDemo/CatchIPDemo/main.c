#define _WINSOCK_DEPRECATED_NO_WARNINGS  //忽略旧API的警告
#include <stdio.h>
#include <WinSock2.h>
#include "IPNode.h"
#pragma comment(lib, "Ws2_32.lib")
//#pragma pack(1)

typedef struct IPHEADER
{
	BYTE Version_HeaderLength;  //版本4位+首部长度4位
	BYTE TypeOfService;  //服务类型
	USHORT TotalLength;  //总长度
	USHORT Identification;  //标识
	USHORT Flags_FragmentOffset;  //标志3位+分片偏移13位
	BYTE TimeToLive;  //生存时间
	BYTE Protocal;  //协议
	USHORT HeaderCheckSum;  //首部校验和
	ULONG SourceAddress;  //源IP地址
	ULONG DestAddress;  //目的IP地址
}IPHEADER, * pIPHEADER;

#define BUFFER_SIZE 65535

void SetWSAIoctl(SOCKET sock)
{
	#define IO_RCVALL _WSAIOW(IOC_VENDOR, 1)
	DWORD dwBufferLen[10];
	DWORD dwBufferInLen = 1;
	DWORD dwBytesReturned = 0;
	WSAIoctl(sock, IO_RCVALL, &dwBufferInLen, sizeof(dwBufferInLen), &dwBufferInLen, sizeof(dwBufferLen), &dwBytesReturned, NULL, NULL);
	printf("%d\n", WSAGetLastError());
};

void PrintList(pIPNODE pIPHeadNode)
{
	if (pIPHeadNode != NULL)
	{
		pIPNODE pTemp = pIPHeadNode;
		printf("node %d\n", pIPHeadNode->node_num);
		while (pTemp)
		{
			UCHAR * sourceIp = (UCHAR *)&pTemp->m_dwSourceIPAddr;
			UCHAR * destIp = (UCHAR *)&pTemp->m_dwDestIPAddr;
			printf("sourceIp:%d.%d.%d.%d \t destIp:%d.%d.%d.%d \t TCP/UDP:%d \t num:%lu\n", sourceIp[0], sourceIp[1], sourceIp[2], sourceIp[3],
				destIp[0], destIp[1], destIp[2], destIp[3], pTemp->m_chProtocol, pTemp->m_dwCouter);
			pTemp = pTemp->pNext;
		}
	}
};

void printIP(ULONG ip)
{
	UCHAR * ipaddr = (UCHAR *)(&ip);
	printf("IP Address: %d.%d.%d.%d\n", ipaddr[0], ipaddr[1], ipaddr[2], ipaddr[3]);
};

int main()
{
	WSADATA wsData;  //初始化Winsock DLL
	WSAStartup(MAKEWORD(2, 2), &wsData);  //2.2版本Socket

	SOCKET sock;
	sock = WSASocket(AF_INET, SOCK_RAW, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);  //创建原始socket，方法过时
	printf("%d\n", WSAGetLastError());

	char localName[256];  //本地机器名
	gethostname(localName, 256);  //获取本机名
	HOSTENT *pHost;  //指向主机信息的指针
	pHost = gethostbyname(localName);  //通过本机名获取本地IP地址，方法过时

	int i = 0;
	while (pHost->h_addr_list[i]!=NULL)
	{
		UCHAR *allIp = (UCHAR *)(pHost->h_addr_list[i]);
		printf("%d: IP address %d.%d.%d.%d \t%s\n", i, allIp[0], allIp[1], allIp[2], allIp[3], pHost->h_name);
		i++;
	}

	int choose = 0;
	printf("choose one IP:\n");
	scanf_s("%d", &choose);

	SOCKADDR_IN addr_in;  //填充sockaddr_in结构
	addr_in.sin_family = AF_INET;  //设定地址类型
	addr_in.sin_port = htons(8000);  //设定端口
	addr_in.sin_addr = *(IN_ADDR *)pHost->h_addr_list[choose];
	bind(sock, (SOCKADDR*)&addr_in, sizeof(addr_in));  //把原始套接字绑定到本地地址上
	printf("%d\n", WSAGetLastError());

	//设置网卡混杂模式
	SetWSAIoctl(sock);

	char buffer[BUFFER_SIZE];
	int catchTime = 100;
	pIPNODE pIPHeadNode = NULL;
	int isHead = 1;
	HOSTENT * pHostent = gethostbyname("www.cqupt.edu.cn");
	printIP((*(IN_ADDR*)pHostent->h_addr).s_addr);

	while (catchTime > 0)
	{
		printf("start\t");
		int nPacketSize = recv(sock, buffer, BUFFER_SIZE, 0);
		printf("%d:%d\n", catchTime, nPacketSize);
		if (nPacketSize > 0)
		{
			pIPHEADER pIpHdr;
			pIpHdr = (pIPHEADER)buffer; 
			//判断IP包的源IP或目的IP是否为本地主机的IP
			if (pIpHdr->SourceAddress == addr_in.sin_addr.S_un.S_addr
				|| pIpHdr->DestAddress == addr_in.sin_addr.S_un.S_addr)
			{
				//统计IP包
				if (isHead) {
					pIPHeadNode = createHead(pIpHdr->SourceAddress, pIpHdr->DestAddress, pIpHdr->Protocal);
					if (pIPHeadNode == NULL) {
						getchar();
						return -1;
					}
					isHead = 0;
				}
				else
				{
					addNode(pIPHeadNode, pIpHdr->SourceAddress, pIpHdr->DestAddress, pIpHdr->Protocal);
				}
			}
			if (pIpHdr->SourceAddress == (*(IN_ADDR*)pHostent->h_addr).s_addr
				&& pIpHdr->DestAddress == addr_in.sin_addr.S_un.S_addr)
			{
				UCHAR * pSourceIp = (UCHAR *)(&pIpHdr->SourceAddress);
				UCHAR * pDestIp = (UCHAR *)(&(pIpHdr->DestAddress));
				printf("%d.%d.%d.%d\t%d.%d.%d.%d\t%d\t", pSourceIp[0], pSourceIp[1], pSourceIp[2], pSourceIp[3],
					pDestIp[0], pDestIp[1], pDestIp[2], pDestIp[3], pIpHdr->Protocal);
				if (pIpHdr->Protocal == 6)
				{
					int totalLen = ntohl(pIpHdr->TotalLength);
					int ipHeadLen = (pIpHdr->Version_HeaderLength & 0x0f) * 4;  //将指针指向tcp头
					UCHAR *pBuffer = ((UCHAR *)buffer) + ipHeadLen;
					unsigned int tcpHdrLen = *(pBuffer + 12);   //将指针指向http头
					tcpHdrLen = (tcpHdrLen >> 4) * 4;
					pBuffer += tcpHdrLen;
					int httpPacketLength = totalLen - ipHeadLen - tcpHdrLen;

					if (httpPacketLength <= 0) {
						continue;
					}
					
					for (int i = 0; i < httpPacketLength; ++i) {
						putchar(*pBuffer);
						if (*pBuffer == '\n') {
							putchar('\t');
						}
						pBuffer++;
					}
					putchar('\n');
				}
			}
			
		}
		catchTime--;
	}
	
	//PrintList(pIPHeadNode);  //打印链表
	
	getchar();
	free(pIPHeadNode);
	return 0;
}

