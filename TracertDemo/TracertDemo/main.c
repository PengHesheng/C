#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>     
#include <ws2tcpip.h>
#include <stdbool.h>
#include "main.h"
#pragma comment (lib, "Ws2_32.lib")

//IPͷ
typedef struct
{
	UCHAR hdr_len : 4;  //4λͷ������
	UCHAR version : 4;  //4λ�汾��
	UCHAR tos;  //8λ��������
	USHORT total_len;  //16λ�ܳ���
	USHORT identifier;  //16λ��ʶ��
	USHORT frag_and_flags;  ///3λ��־+13λƬλ��
	UCHAR ttl;  //8λ����ʱ��
	UCHAR protocol;  //8λ�ϲ�Э���
	USHORT checksum;  //16λУ���
	ULONG sourceIP;  //ԴIP
	ULONG destIP;  //Ŀ��IP
} IP_HEADER, * pIP_HEADER;

//ICMPͷ
typedef struct
{
	BYTE type;  //8λ�����ֶ�
	BYTE code;  //8λ�����ֶ�
	USHORT checksum;  //16λУ���
	USHORT id;  //16λ��ʶ��
	USHORT seq;  //16λ���к�
} ICMP_HEADER, * pICMP_HEADER;

//���ݸ����뺯���Ľṹ��
typedef struct
{
	USHORT usSeqNo;  //���к�  in
	DWORD dwRoundTripTime;  //����ʱ��  in/out
	IN_ADDR dwIPaddr;  //���ر��ĵ�IP  out
} DECODE_RESULT;

USHORT checkSum(USHORT * pBuf, int iSize);
BOOL DecodeIcmpResponse(char * pBuf, int iPacketSize, DECODE_RESULT * decodeResult);

void printIP(ULONG ip)
{
	UCHAR * ipaddr = (UCHAR *)(&ip);
	printf("IP Address: %d.%d.%d.%d", ipaddr[0], ipaddr[1], ipaddr[2], ipaddr[3]);
};

//ICMP�����ֶ�
const BYTE ICMP_ECHO_REQUEST = 8;  //����
const BYTE ICMP_ECHO_REPLY = 0;  //Ӧ��
const BYTE ICMP_TIMEOUT = 11;  //��ʱ�����

//��������
#define DEF_ICMP_DATA_SIZE 32  //ICMPĬ�����ݳ���
#define MAX_ICMP_PACKET_SIZE 1024  //ICMP����ĳ��ȣ�������ͷ��
const DWORD DEF_ICMP_TIMEOUT = 3000;  //Ӧ��ʱʱ��ms
const int DEF_MAX_HOP = 30;  //�������

int main(int arg, char * argv[])
{
	//char * destIp = "192186213";
	char * destIp = "www.baidu.com";
	//��ʼ��Sokcet����
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	printf("start %d\n", WSAGetLastError());

	u_long ulDestIP = inet_addr(destIp);
	if (ulDestIP == INADDR_NONE)
	{
		//ת�����ɹ�ʱ����������
		HOSTENT * pHostent = gethostbyname(destIp);
		printf("gethostbyname %d\n", WSAGetLastError());
		if (pHostent)
		{
			ulDestIP = (*(IN_ADDR*)pHostent->h_addr).s_addr;
			printIP(ulDestIP);
		}
	}
	else
	{
		WSACleanup();
		getchar();
		return -1;
	}

	//���Ŀ�Ķ�Socket��ַ
	SOCKADDR_IN destSockAddr;
	ZeroMemory(&destSockAddr, sizeof(SOCKADDR_IN));
	destSockAddr.sin_family = AF_INET;
	destSockAddr.sin_addr.s_addr = ulDestIP;

	SOCKET sockRaw = WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0, WSA_FLAG_OVERLAPPED);
	printf("\ncreate %d\n", WSAGetLastError());

	int iTimeout = 3000;  //3s
	setsockopt(sockRaw, SOL_SOCKET, SO_RCVTIMEO, (char*)&iTimeout, sizeof(iTimeout));  //���ý��ճ�ʱ
	setsockopt(sockRaw, SOL_SOCKET, SO_SNDTIMEO, (char*)&iTimeout, sizeof(iTimeout));  //���÷��ͳ�ʱ

	char IcmpSendBuf[sizeof(ICMP_HEADER) + DEF_ICMP_DATA_SIZE];  //���ͻ�����
	memset(IcmpSendBuf, 0, sizeof(IcmpSendBuf));  //��ʼ�����ͻ�����
	char IcmpRcvBuf[MAX_ICMP_PACKET_SIZE];  //���ջ�����
	memset(IcmpRcvBuf, 0, sizeof(IcmpRcvBuf) );   //��ʼ�����ջ�����

	pICMP_HEADER pIcmpHeader = (pICMP_HEADER)IcmpSendBuf;
	pIcmpHeader->type = ICMP_ECHO_REQUEST;
	pIcmpHeader->code = 0;
	pIcmpHeader->id = (USHORT)GetCurrentProcessId();  //��ȡ��ǰ����ID
	memset(IcmpSendBuf + sizeof(ICMP_HEADER), 'E', DEF_ICMP_DATA_SIZE);  //�����ֶ�

	USHORT usSeqNo = 0;  //���к�
	int iTTL = 1;  //TTL
	BOOL bReachDestHost = FALSE;  //ѭ���˳���־
	int iMaxHop = DEF_MAX_HOP;
	DECODE_RESULT decodeResult;
	while (!bReachDestHost && iMaxHop--)
	{
		//����IPͷ��TTL
		setsockopt(sockRaw, IPPROTO_IP, IP_TTL, (char*)&iTTL, sizeof(iTTL));
		printf("TTL %d: ", iTTL);
		//���ICMP�ֶ�
		((pICMP_HEADER)IcmpSendBuf)->checksum = 0;
		((pICMP_HEADER)IcmpSendBuf)->seq = htons(usSeqNo++);
		((pICMP_HEADER)IcmpSendBuf)->checksum = checkSum((USHORT*)IcmpSendBuf, sizeof(ICMP_HEADER) + DEF_ICMP_DATA_SIZE);  //����У���

		//��¼��ǰ���кź�ʱ��
		decodeResult.usSeqNo = ((pICMP_HEADER)IcmpSendBuf)->seq;
		decodeResult.dwRoundTripTime = GetTickCount();  //��ȡ��ǰʱ�䣬��ʵʱ

		//����ICMP����
		sendto(sockRaw, IcmpSendBuf, sizeof(IcmpSendBuf), 0, (SOCKADDR*)&destSockAddr, sizeof(destSockAddr));
		printf("\tsendto: %d", WSAGetLastError());
		//���մ���
		SOCKADDR_IN from;  //�Զ�Socket
		int iFromLen = sizeof(from);  //��ַ�ṹ��С
		int iReadDataLen;  //�������ݳ���
		while (1)
		{
			iReadDataLen = recvfrom(sockRaw, IcmpRcvBuf, MAX_ICMP_PACKET_SIZE, 0, (SOCKADDR*)&from, &iFromLen);
			printf("\tiReadDataLen%d\t%d\t", iReadDataLen, WSAGetLastError());
			if (iReadDataLen != SOCKET_ERROR)
			{
				//�����ݰ�����
				if (DecodeIcmpResponse(IcmpRcvBuf, iReadDataLen, &decodeResult))
				{
					
					//����Ŀ�ĵ�
					if (decodeResult.dwIPaddr.s_addr == destSockAddr.sin_addr.s_addr)
					{
						bReachDestHost = TRUE;
						UCHAR * IpAddr = (UCHAR *)&decodeResult.dwIPaddr.s_addr;
						printf("\tDest IP Address: %d.%d.%d.%d\n", IpAddr[0], IpAddr[1], IpAddr[2], IpAddr[3]);
						//break;
					}
					printIP(decodeResult.dwIPaddr.s_addr);
					printf("\n");
					break;
				}
			}
			else if (WSAGetLastError() == WSAETIMEDOUT)  //���ճ�ʱ
			{
				printf("*\trecv time out\n");
				break;
			}
			else
			{
				printf("fun error\n");
				break;
			}
		}
		iTTL++;
	}
	getchar();
	return 0;
}

//����У���
USHORT checkSum(USHORT * pBuf, int iSize)
{
	ULONG cksum = 0;
	//ż�����
	while (iSize > 1)
	{
		cksum += *pBuf++;  //��2���ֽ�Ϊһ�������������
		iSize -= sizeof(USHORT);
	}
	//�������
	if (iSize)
	{
		cksum += *(UCHAR*)pBuf;
	}
	cksum = (cksum >> 16) + (cksum & 0xffff);  //ȡ��λ���16λ���
	cksum += (cksum >> 16);  //��ֹ�ٴβ�����λ����λ��
	return (USHORT)(~cksum);  //һ��Ҫȡ������Ȼ�������һ���޷����գ�����Ŀ�ĵغ��޷�������Ӧ��
}

BOOL DecodeIcmpResponse(char * pBuf, int iPacketSize, DECODE_RESULT * decodeResult)
{
	int iIpHdrLen = ((IP_HEADER *)pBuf)->hdr_len * 4;  //IPͷ��
	//����ICMP����������ȡID�ֶκ����к�
	pICMP_HEADER pIcmpHdr = (pICMP_HEADER)(pBuf + iIpHdrLen);
	USHORT usID, usSquNo;
	if (pIcmpHdr->type == ICMP_ECHO_REPLY)
	{
		usID = pIcmpHdr->id;
		usSquNo = pIcmpHdr->seq;
	}
	else if (pIcmpHdr->type == ICMP_TIMEOUT)
	{
		char* pInnerIpHdr = pBuf + iIpHdrLen + sizeof(ICMP_HEADER);  //�����е�IPͷ
		int iInnerIPHdrLen = ((pIP_HEADER)pInnerIpHdr)->hdr_len * 4;  //�����е�IPͷ��
		pICMP_HEADER pInnerIcmpHdr = (pICMP_HEADER)(pInnerIpHdr + iInnerIPHdrLen);  //�����е�ICMP��ͷ
		usID = pInnerIcmpHdr->id;
		usSquNo = pInnerIcmpHdr->seq;
	}
	else
	{
		return FALSE;
	}

	//����
	if (usID != (USHORT)GetCurrentProcessId() || usSquNo != decodeResult->usSeqNo)
	{
		return FALSE;
	}

	//��¼IP�ͼ�������ʱ��
	decodeResult->dwIPaddr.s_addr = ((pIP_HEADER)pBuf)->sourceIP;
	decodeResult->dwRoundTripTime = GetTickCount() - decodeResult->dwRoundTripTime;

	if (decodeResult->dwRoundTripTime)
	{
		printf("%6dms\t", decodeResult->dwRoundTripTime);
	}
	else
	{
		printf("1ms\t");
	}
	return TRUE;
}