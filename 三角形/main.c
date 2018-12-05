#include <stdio.h>
#include <math.h>
#define EPS 1e-1
int main()
{
    float a,b,c;
    int flag=1;   /*置标志变量为非0值*/
    printf("Input a,b,c:");
    scanf("%f,%f,%f", &a, &b, &c);
    if(a+b>c && b+c>a && a+c>b)  /*如果满足三角形的基本条件*/
    {
        if(fabs(a-b)<=EPS&&fabs(b-c)<=EPS&&fabs(c-a)<=EPS)
        {
            printf("等边");
            flag=0;  /*置标志变量为0值*/
        }
        else if(fabs(a-b)<=EPS||fabs(b-c)<=EPS||fabs(c-a)<=EPS)
        {
            printf("等腰");
            flag=0;  /*置标志变量为0值*/
        }
        if(fabs(a*a+b*b-c*c)<=EPS||fabs(a*a+c*c-b*b)<=EPS||fabs(b*b+c*c-a*a)<=EPS)
        {
            printf("直角");
            flag=0;  /*置标志变量为0值*/
        }
        if(flag) /*若flag为非0，则是一般三角形*/
        {
            printf("一般");
        }
        printf("三角形\n");
    }
    else  /*如果不满足*/
    {
        printf("不是\n");
    }
    return 0;
}
