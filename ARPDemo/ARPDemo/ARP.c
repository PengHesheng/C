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
		ULONG ulHostIp = ntohl(pIpAddrTable->table[choose].dwAddr);  //获取主机IP		
		ULONG ulHostMask = ntohl(pIpAddrTable->table[choose].dwMask);  //获取主机子网掩码

		//遍历子网内所有主机
		for (ULONG i = 1; i < (~ulHostMask); i++)
		{
			static ULONG uNo = 0;
			HRESULT hr;
			IPAddr ipAddr;
			ULONG pulMac[2];  //存放MAC地址
			ULONG ulLen;  //MAC地址长度
			ipAddr = htonl(i + (ulHostIp & ulHostMask));  //主机到网络字节序
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

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
