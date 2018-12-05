#include <WinSock2.h>
#include <iphlpapi.h>
#include <tchar.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#pragma comment (lib, "iphlpapi.lib")
#pragma comment (lib, "ws2_32.lib")

int main()
{
	MIB_IPADDRTABLE *pIpAddrTable = (MIB_IPADDRTABLE *)malloc(sizeof(MIB_IPADDRROW));
	ULONG dwSize = 0, dwRetVal = 0;
	if (GetIpAddrTable(pIpAddrTable, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER)
	{
		free(pIpAddrTable);
		pIpAddrTable = (MIB_IPADDRTABLE *)malloc(sizeof(dwSize));
	}

	if ((dwRetVal = GetIpAddrTable(pIpAddrTable, &dwSize, 0)) == NO_ERROR)
	{
		int i = 0;
		while (i < pIpAddrTable->dwNumEntries)
		{
			ULONG hostIp = ntohl(pIpAddrTable->table[i].dwAddr);
			IPAddr allIpAddr = htonl(hostIp);
			unsigned char *allIp = (unsigned char *)(&allIpAddr);
			printf("IP address %d.%d.%d.%d \n", allIp[0], allIp[1], allIp[2], allIp[3]);
			i++;
		}
		int choose;
		printf("choose one IP:\n");
		scanf_s("%d", &choose);
		ULONG ulHostIp = ntohl(pIpAddrTable->table[choose].dwAddr);  //��ȡ����IP		
		ULONG ulHostMask = ntohl(pIpAddrTable->table[choose].dwMask);  //��ȡ������������

		//������������������
		for (ULONG i = 1; i < (~ulHostMask); i++)
		{
			static ULONG uNo = 0;
			HRESULT hr;
			IPAddr ipAddr;
			ULONG pulMac[2];  //���MAC��ַ
			ULONG ulLen;  //MAC��ַ����
			ipAddr = htonl(i + (ulHostIp & ulHostMask));  //�����������ֽ���
			memset(pulMac, 0xff, sizeof(pulMac));
			ulLen = 6;
			hr = SendARP(ipAddr, 0, pulMac, &ulLen);
			if (ulLen == 6)
			{
				uNo++;
				PBYTE pbHexMac = (PBYTE) pulMac;
				unsigned char *strIpAddr = (unsigned char *)(&ipAddr);
				printf("%d:MAC address %02X:%02X:%02X:%02X:%02X:%02X  IP address %d.%d.%d.%d \n", 
					uNo, pbHexMac[0], pbHexMac[1], pbHexMac[2], pbHexMac[3], pbHexMac[4], pbHexMac[5], strIpAddr[0], strIpAddr[1], strIpAddr[2], strIpAddr[3]);
			}
		}
	}
	else
	{
		printf("failed\n");
	}
	printf("over\n");
	free(pIpAddrTable);
	getchar();
	return 0;
}

// ���г���: Ctrl + F5 ����� >����ʼִ��(������)���˵�
// ���Գ���: F5 ����� >����ʼ���ԡ��˵�

// ������ʾ: 
//   1. ʹ�ý��������Դ�������������/�����ļ�
//   2. ʹ���Ŷ���Դ�������������ӵ�Դ�������
//   3. ʹ��������ڲ鿴���������������Ϣ
//   4. ʹ�ô����б��ڲ鿴����
//   5. ת������Ŀ��>���������Դ����µĴ����ļ�����ת������Ŀ��>�����������Խ����д����ļ���ӵ���Ŀ
//   6. ��������Ҫ�ٴδ򿪴���Ŀ����ת�����ļ���>���򿪡�>����Ŀ����ѡ�� .sln �ļ�
