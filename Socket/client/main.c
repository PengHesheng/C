#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{
    //沉睡2秒再连接server
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
           recv函数中的bufferlength参数是可以固定值的
           send函数中的bufferlength参数不能固定值，需要看实际长度，并且考虑到'\0'字符串
        */
    }

    closesocket(s);

    WSACleanup();

    getchar();

    return 0;
}
