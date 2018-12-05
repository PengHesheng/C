#include <stdio.h>
#include <string.h>
void SortString(char str[][10], int n)
{
    int i,j;
    char temp[10];
    for (i=0; i<n-1; i++)
    {
        for (j=i+1; j<n; j++)
        {
            if (strcmp(str[j],str[i]) < 0) //一个一个比较，排序
            {
                //实现字符串的交换
                strcpy(temp,str[i]);
                strcpy(str[i],str[j]);
                strcpy(str[j],temp);
            }
        }
    }
}
int main()
{
    int i,n;
    char name[150][10];
    scanf("%d", &n);
    getchar();
    for (i=0; i<n; i++)
        gets(name[i]);
    SortString(name,n);
    printf("\n");
    for (i=0; i<n; i++)
        puts(name[i]);
    return 0;
}
