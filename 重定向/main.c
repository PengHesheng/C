#include <stdio.h>
#include <stdlib.h>
int main()
{
    int i;
    if (freopen("F:\\C\\output.txt","w",stdout) == NULL) //����ܹ��ض��򣬷����ļ�ָ�룬���򷵻�NULL
        fprintf(stderr,"error stdout");
    for (i=0; i<10; i++)
        printf("%3d",i);  //������ļ���
    printf("kjl\n");
    fclose(stdout);
    return 0;
}
