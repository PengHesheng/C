#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{
    //��˯2��������server
    Sleep(2000);

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in sockaddr;
    sockaddr.sin_family = PF_INET;
    sockaddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    sockaddr.sin_port = htons(8000);

    connect(s, (SOCKADDR*) &sockaddr, sizeof(SOCKADDR));

    char buffer[MAXBYTE] = {0};
    recv(s, buffer, MAXBYTE, NULL);
    printf("***SERVER***%s", buffer);

    while(TRUE)
    {
        char * myMsg[100000];
        printf("You can chat with server now:\n");
        gets(myMsg);
        send(s, myMsg, strlen(myMsg) + sizeof(char), NULL);
         /*
           recv�����е�bufferlength�����ǿ��Թ̶�ֵ��
           send�����е�bufferlength�������̶ܹ�ֵ����Ҫ��ʵ�ʳ��ȣ����ҿ��ǵ�'\0'�ַ���
        */
    }

    closesocket(s);

    WSACleanup();

    getchar();

    return 0;
}
