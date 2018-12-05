#define _WINSOCK_DEPRECATED_NO_WARNINGS  //���Ծ�API�ľ���
#include <stdio.h>
#include <WinSock2.h>
#include "IPNode.h"
#pragma comment(lib, "Ws2_32.lib")
//#pragma pack(1)

typedef struct IPHEADER
{
	BYTE Version_HeaderLength;  //�汾4λ+�ײ�����4λ
	BYTE TypeOfService;  //��������
	USHORT TotalLength;  //�ܳ���
	USHORT Identification;  //��ʶ
	USHORT Flags_FragmentOffset;  //��־3λ+��Ƭƫ��13λ
	BYTE TimeToLive;  //����ʱ��
	BYTE Protocal;  //Э��
	USHORT HeaderCheckSum;  //�ײ�У���
	ULONG SourceAddress;  //ԴIP��ַ
	ULONG DestAddress;  //Ŀ��IP��ַ
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
	WSADATA wsData;  //��ʼ��Winsock DLL
	WSAStartup(MAKEWORD(2, 2), &wsData);  //2.2�汾Socket

	SOCKET sock;
	sock = WSASocket(AF_INET, SOCK_RAW, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);  //����ԭʼsocket��������ʱ
	printf("%d\n", WSAGetLastError());

	char localName[256];  //���ػ�����
	gethostname(localName, 256);  //��ȡ������
	HOSTENT *pHost;  //ָ��������Ϣ��ָ��
	pHost = gethostbyname(localName);  //ͨ����������ȡ����IP��ַ��������ʱ

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

	SOCKADDR_IN addr_in;  //���sockaddr_in�ṹ
	addr_in.sin_family = AF_INET;  //�趨��ַ����
	addr_in.sin_port = htons(8000);  //�趨�˿�
	addr_in.sin_addr = *(IN_ADDR *)pHost->h_addr_list[choose];
	bind(sock, (SOCKADDR*)&addr_in, sizeof(addr_in));  //��ԭʼ�׽��ְ󶨵����ص�ַ��
	printf("%d\n", WSAGetLastError());

	//������������ģʽ
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
			//�ж�IP����ԴIP��Ŀ��IP�Ƿ�Ϊ����������IP
			if (pIpHdr->SourceAddress == addr_in.sin_addr.S_un.S_addr
				|| pIpHdr->DestAddress == addr_in.sin_addr.S_un.S_addr)
			{
				//ͳ��IP��
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
					int ipHeadLen = (pIpHdr->Version_HeaderLength & 0x0f) * 4;  //��ָ��ָ��tcpͷ
					UCHAR *pBuffer = ((UCHAR *)buffer) + ipHeadLen;
					unsigned int tcpHdrLen = *(pBuffer + 12);   //��ָ��ָ��httpͷ
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
	
	//PrintList(pIPHeadNode);  //��ӡ����
	
	getchar();
	free(pIPHeadNode);
	return 0;
}

