#include <stdio.h>
int main()
{
    int a[5] = {},n,i,q,w;
    scanf("%d", &n);
    q = n;
    w = 1;
    while(q >= 10)
    {
        w++;
        q = q / 10;
    }   //判断位数
    printf("%d\n",w);
    for (i = 0;n > 0;i++)
    {
        a[i] = n % 10;
        n = n / 10;
    }    //将每一位存在数组中
    for (i = w - 1;i >= 0;i--)
    {
        printf("%d",a[i]);
        if (i)
            printf(" ");
    }
    printf("\n");
    for (i = 0;i <= w - 1;i++)
    {
        if (i)  printf(" ");
            printf("%d",a[i]);
    }
    printf("\n");
    return 0;
}
