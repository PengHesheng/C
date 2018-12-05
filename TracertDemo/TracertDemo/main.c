#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>     
#include <ws2tcpip.h>
#include <stdbool.h>
#include "main.h"
#pragma comment (lib, "Ws2_32.lib")

//IP头
typedef struct
{
	UCHAR hdr_len : 4;  //4位头部长度
	UCHAR version : 4;  //4位版本号
	UCHAR tos;  //8位服务类型
	USHORT total_len;  //16位总长度
	USHORT identifier;  //16位标识符
	USHORT frag_and_flags;  ///3位标志+13位片位移
	UCHAR ttl;  //8位生存时间
	UCHAR protocol;  //8位上层协议号
	USHORT checksum;  //16位校验和
	ULONG sourceIP;  //源IP
	ULONG destIP;  //目的IP
} IP_HEADER, * pIP_HEADER;

//ICMP头
typedef struct
{
	BYTE type;  //8位类型字段
	BYTE code;  //8位代码字段
	USHORT checksum;  //16位校验和
	USHORT id;  //16位标识符
	USHORT seq;  //16位序列号
} ICMP_HEADER, * pICMP_HEADER;

//传递给解码函数的结构体
typedef struct
{
	USHORT usSeqNo;  //序列号  in
	DWORD dwRoundTripTime;  //往返时间  in/out
	IN_ADDR dwIPaddr;  //返回报文的IP  out
} DECODE_RESULT;

USHORT checkSum(USHORT * pBuf, int iSize);
BOOL DecodeIcmpResponse(char * pBuf, int iPacketSize, DECODE_RESULT * decodeResult);

void printIP(ULONG ip)
{
	UCHAR * ipaddr = (UCHAR *)(&ip);
	printf("IP Address: %d.%d.%d.%d", ipaddr[0], ipaddr[1], ipaddr[2], ipaddr[3]);
};

//ICMP类型字段
const BYTE ICMP_ECHO_REQUEST = 8;  //请求
const BYTE ICMP_ECHO_REPLY = 0;  //应答
const BYTE ICMP_TIMEOUT = 11;  //超时差错报文

//其他常量
#define DEF_ICMP_DATA_SIZE 32  //ICMP默认数据长度
#define MAX_ICMP_PACKET_SIZE 1024  //ICMP最大报文长度（包含报头）
const DWORD DEF_ICMP_TIMEOUT = 3000;  //应答超时时间ms
const int DEF_MAX_HOP = 30;  //最大跳数

int main(int arg, char * argv[])
{
	//char * destIp = "192186213";
	char * destIp = "www.baidu.com";
	//初始化Sokcet环境
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	printf("start %d\n", WSAGetLastError());

	u_long ulDestIP = inet_addr(destIp);
	if (ulDestIP == INADDR_NONE)
	{
		//转换不成功时按域名解析
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

	//填充目的端Socket地址
	SOCKADDR_IN destSockAddr;
	ZeroMemory(&destSockAddr, sizeof(SOCKADDR_IN));
	destSockAddr.sin_family = AF_INET;
	destSockAddr.sin_addr.s_addr = ulDestIP;

	SOCKET sockRaw = WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0, WSA_FLAG_OVERLAPPED);
	printf("\ncreate %d\n", WSAGetLastError());

	int iTimeout = 3000;  //3s
	setsockopt(sockRaw, SOL_SOCKET, SO_RCVTIMEO, (char*)&iTimeout, sizeof(iTimeout));  //设置接收超时
	setsockopt(sockRaw, SOL_SOCKET, SO_SNDTIMEO, (char*)&iTimeout, sizeof(iTimeout));  //设置发送超时

	char IcmpSendBuf[sizeof(ICMP_HEADER) + DEF_ICMP_DATA_SIZE];  //发送缓冲区
	memset(IcmpSendBuf, 0, sizeof(IcmpSendBuf));  //初始化发送缓冲区
	char IcmpRcvBuf[MAX_ICMP_PACKET_SIZE];  //接收缓冲区
	memset(IcmpRcvBuf, 0, sizeof(IcmpRcvBuf) );   //初始化接收缓冲区

	pICMP_HEADER pIcmpHeader = (pICMP_HEADER)IcmpSendBuf;
	pIcmpHeader->type = ICMP_ECHO_REQUEST;
	pIcmpHeader->code = 0;
	pIcmpHeader->id = (USHORT)GetCurrentProcessId();  //获取当前进程ID
	memset(IcmpSendBuf + sizeof(ICMP_HEADER), 'E', DEF_ICMP_DATA_SIZE);  //数据字段

	USHORT usSeqNo = 0;  //序列号
	int iTTL = 1;  //TTL
	BOOL bReachDestHost = FALSE;  //循环退出标志
	int iMaxHop = DEF_MAX_HOP;
	DECODE_RESULT decodeResult;
	while (!bReachDestHost && iMaxHop--)
	{
		//设置IP头的TTL
		setsockopt(sockRaw, IPPROTO_IP, IP_TTL, (char*)&iTTL, sizeof(iTTL));
		printf("TTL %d: ", iTTL);
		//填充ICMP字段
		((pICMP_HEADER)IcmpSendBuf)->checksum = 0;
		((pICMP_HEADER)IcmpSendBuf)->seq = htons(usSeqNo++);
		((pICMP_HEADER)IcmpSendBuf)->checksum = checkSum((USHORT*)IcmpSendBuf, sizeof(ICMP_HEADER) + DEF_ICMP_DATA_SIZE);  //计算校验和

		//记录当前序列号和时间
		decodeResult.usSeqNo = ((pICMP_HEADER)IcmpSendBuf)->seq;
		decodeResult.dwRoundTripTime = GetTickCount();  //获取当前时间，非实时

		//发送ICMP请求
		sendto(sockRaw, IcmpSendBuf, sizeof(IcmpSendBuf), 0, (SOCKADDR*)&destSockAddr, sizeof(destSockAddr));
		printf("\tsendto: %d", WSAGetLastError());
		//接收处理
		SOCKADDR_IN from;  //对端Socket
		int iFromLen = sizeof(from);  //地址结构大小
		int iReadDataLen;  //接收数据长度
		while (1)
		{
			iReadDataLen = recvfrom(sockRaw, IcmpRcvBuf, MAX_ICMP_PACKET_SIZE, 0, (SOCKADDR*)&from, &iFromLen);
			printf("\tiReadDataLen%d\t%d\t", iReadDataLen, WSAGetLastError());
			if (iReadDataLen != SOCKET_ERROR)
			{
				//对数据包解码
				if (DecodeIcmpResponse(IcmpRcvBuf, iReadDataLen, &decodeResult))
				{
					
					//到达目的地
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
			else if (WSAGetLastError() == WSAETIMEDOUT)  //接收超时
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

//计算校验和
USHORT checkSum(USHORT * pBuf, int iSize)
{
	ULONG cksum = 0;
	//偶数相加
	while (iSize > 1)
	{
		cksum += *pBuf++;  //以2个字节为一个整数进行相加
		iSize -= sizeof(USHORT);
	}
	//奇数相加
	if (iSize)
	{
		cksum += *(UCHAR*)pBuf;
	}
	cksum = (cksum >> 16) + (cksum & 0xffff);  //取高位与低16位相加
	cksum += (cksum >> 16);  //防止再次产生进位（高位）
	return (USHORT)(~cksum);  //一定要取反，不然导致最后一跳无法接收（到达目的地后无法接收响应）
}

BOOL DecodeIcmpResponse(char * pBuf, int iPacketSize, DECODE_RESULT * decodeResult)
{
	int iIpHdrLen = ((IP_HEADER *)pBuf)->hdr_len * 4;  //IP头长
	//根据ICMP报文类型提取ID字段和序列号
	pICMP_HEADER pIcmpHdr = (pICMP_HEADER)(pBuf + iIpHdrLen);
	USHORT usID, usSquNo;
	if (pIcmpHdr->type == ICMP_ECHO_REPLY)
	{
		usID = pIcmpHdr->id;
		usSquNo = pIcmpHdr->seq;
	}
	else if (pIcmpHdr->type == ICMP_TIMEOUT)
	{
		char* pInnerIpHdr = pBuf + iIpHdrLen + sizeof(ICMP_HEADER);  //数据中的IP头
		int iInnerIPHdrLen = ((pIP_HEADER)pInnerIpHdr)->hdr_len * 4;  //数据中的IP头长
		pICMP_HEADER pInnerIcmpHdr = (pICMP_HEADER)(pInnerIpHdr + iInnerIPHdrLen);  //数据中的ICMP报头
		usID = pInnerIcmpHdr->id;
		usSquNo = pInnerIcmpHdr->seq;
	}
	else
	{
		return FALSE;
	}

	//检验
	if (usID != (USHORT)GetCurrentProcessId() || usSquNo != decodeResult->usSeqNo)
	{
		return FALSE;
	}

	//记录IP和计算往返时间
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