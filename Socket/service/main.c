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
    //��Ҫ�󶨵����ص�IP��ַ
    sockaddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    //��Ҫ�󶨵Ķ˿�
    sockaddr.sin_port = htons(8000);
    //���а󶨶���
    bind(s, (SOCKADDR*) &sockaddr, sizeof(SOCKADDR));
    //��������
    listen(s, 1);

    printf("listening on port [%d] . \n", 8000);

    while(TRUE)
    {
        SOCKADDR clientAddr;
        int size  = sizeof(SOCKADDR);

        SOCKET clientSocket;
        //������ֱ������top�ͻ�������
        clientSocket = accept(s, &clientAddr, &size);
        printf("***SYS***    New Client touched.\n");

        char * msg = "Hello, my client.\r\n";
        //����ĵ���������Ҫע�⣬�Ǽ���һ��char���ȵ�
        send(clientSocket, msg, strlen(msg) + sizeof(char), NULL);
        printf("***SYS***    Hello.\n");

        while(TRUE)
        {
            char buffer[MAXBYTE] = {0};
            //һֱ���ܿͻ���Socket��send����
            recv(clientSocket, buffer, MAXBYTE, NULL);
            printf("***Client***    %s\n", buffer);
        }

        //�ر�Socket
        closesocket(clientSocket);
    }
    //�رռ���socket
    closesocket(s);

    //ж��
    WSACleanup();

    getchar();

    return 0;
}
