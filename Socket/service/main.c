#include <stdio.h>
#include <stdlib.h>
#include <Winsock2.h>
#include "WS2_32.Lib"

int main()
{
    //service

    int err;
    WSADATA wsaDara;
    err = WSAStartup(MAKEWORD(2,2), &wsaDara);
    if(err != 0)
    {
        return;
    }

    SOCKET s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in sockaddr;
    sockaddr.sin_family = PF_INET;
    //需要绑定到本地的IP地址
    sockaddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    //需要绑定的端口
    sockaddr.sin_port = htons(8000);
    //进行绑定动作
    bind(s, (SOCKADDR*) &sockaddr, sizeof(SOCKADDR));
    //启动监听
    listen(s, 1);

    printf("listening on port [%d] . \n", 8000);

    while(TRUE)
    {
        SOCKADDR clientAddr;
        int size  = sizeof(SOCKADDR);

        SOCKET clientSocket;
        //阻塞，直到有新top客户端连接
        clientSocket = accept(s, &clientAddr, &size);
        printf("***SYS***    New Client touched.\n");

        char * msg = "Hello, my client.\r\n";
        //这里的第三个参数要注意，是加了一个char长度的
        send(clientSocket, msg, strlen(msg) + sizeof(char), NULL);
        printf("***SYS***    Hello.\n");

        while(TRUE)
        {
            char buffer[MAXBYTE] = {0};
            //一直接受客户端Socket的send操作
            recv(clientSocket, buffer, MAXBYTE, NULL);
            printf("***Client***    %s\n", buffer);
        }

        //关闭Socket
        closesocket(clientSocket);
    }
    //关闭监听socket
    closesocket(s);

    //卸载
    WSACleanup();

    getchar();

    return 0;
}
