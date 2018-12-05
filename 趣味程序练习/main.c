#include <stdio.h>
#include <math.h>


/*
//Ñî»ÔÈý½Ç
int c(int x,int y)
{
    int z;
    if ((y==1)||(y==x+1))
        return 1;
    z=c(x-1,y-1)+c(x-1,y);
    return z;
}
int main()
{
    int i,j,n=13;
    while(n>12)
        scanf("%d",&n);
    for (i=0; i<=n; i++)
    {
        for (j=0;j<24-2*i;j++)
            printf(" ");
        for (j=1;j<i+2;j++)
            printf("%4d",c(i,j));
        printf("\n");
    }
}

int main()
{
    int i,x,y,last=1;
    scanf("%d %d", &x, &y);
    for (i=1; i<=y; i++)
    {
        last=last*x%1000;
    }
    printf("%d",last%1000);
    return 0;
}

//ÓàÏÒº¯Êý

int main()
{
    double y;
    int x,m;
    for (y=1; y >= -1; y -= 0.1)
    {
        m=acos(y)*10;
        for (x=1; x<m; x++)
            printf(" ");
        printf("*");
        for (x=1; x<32-m; x++)
        {
            printf("  ");
        }
        printf("*\n");
    }
    printf("%d\n",m);
    return 0;
}

int main()
{
    int max,min,i;
    max=0;
    min=100;
    int a[10]={1,2,3,6,4,5,8,9,0,0};
    for (i=0; i<10; i++)
    {
        if (a[i]>max)
            max=a[i];
        if (a[i]<min)
            min=a[i];
    }
    printf(",max=%d,min=%d",max,min);
    return 0;
}*/

