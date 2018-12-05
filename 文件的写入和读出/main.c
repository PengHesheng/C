//字符串的写入和读出
#include <stdio.h>
#include <stdlib.h>
#define N 80000
int main()
{
    FILE *fp;
    char str[N];
    /*if ((fp=fopen("F:\\C\\demo2.txt","a")) == NULL) //检查是否打开文件失败
    {
        printf("Failure\n");
        exit(0);   //用exit来终止程序
    }
    gets(str);
    fputs(str,fp);
    fclose(fp);*/
    if ((fp=fopen("F:\\C\\demo2.txt","r")) == NULL)
    {
        printf("Failure!\n");
        exit(0);
    }
    fgets(str,N,fp);
    puts(str);
    fclose(fp);
    return 0;
}





//文件的二进制字符写入和读出
/*#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
int main()
{
    int i;
    char ch;
    FILE *fp;
    fp=fopen("F:\\C\\demo.bin","wb");  //以二进制写方式打开文件（以二进制的形式写入）
    if ((fp == NULL))
    {
        printf("Failure\n");
        exit(0);
    }
    for (i=0; i<128; i++)
        fputc(i,fp);   //将字符的ASCII码值写入文件
    fclose(fp);
    fp=fopen("F:\\C\\demo.bin","rb");   //以二进制读方式打开文件（以二进制的形式读出，显示）
    if (fp == NULL)
    {
        printf("Failure\n");
        exit(0);
    }
    while((ch = fgetc(fp)) != EOF)
    {
        if (isprint(ch))  //判断是否为可打印字符
            printf("%c\t",ch);
        else
            printf("%d\t",ch);
    }   //判断是否都到文件末尾的两种方法
    //ch=fgetc(fp);
    //while (!feof(fp))  //从文件中读取字符直到文件末尾
    //{
        //ch=fgetc(fp);
        //if (!iscntrl(ch))   //判断是否为可控制字符      //if (isprint(ch))  //判断是否为可打印字符
            //printf("%c\t",ch);
        //else
            //printf("%d\t",ch);
    //}
    fclose(fp);
    return 0;
}*/




/*#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#define FILE_PATH "F:/demo.txt"
int main()
{
    char name[20];
    int age;
    float score;
    int stuNo;
    FILE *fp;
    if ((fp = fopen(FILE_PATH,"wt+")) == NULL)
    {
        perror(FILE_PATH);
        exit(1);
    }
    printf("ÇëÊäÈë");
    while(scanf("%s %d %d %f", name, &age, &stuNo, &score) != EOF)
    {
        fprintf(fp,"%s\t%d\t%d\t%f\n",name,age,stuNo,score);
    }
    fflush(fp);
    rewind(fp);
    printf("\n¶ÁÈ¡ÄÚÈÝ:\n");
    while(fscanf(fp,"%s\t%d\t%d\t%f",name, &age, &stuNo, &score) != EOF)
    {
        printf("%s %d %d %f\n",name,age,stuNo,score);
    }
    fclose(fp);
    return EXIT_SUCCESS;
}*/
