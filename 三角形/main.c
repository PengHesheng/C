#include <stdio.h>
#include <math.h>
#define EPS 1e-1
int main()
{
    float a,b,c;
    int flag=1;   /*�ñ�־����Ϊ��0ֵ*/
    printf("Input a,b,c:");
    scanf("%f,%f,%f", &a, &b, &c);
    if(a+b>c && b+c>a && a+c>b)  /*������������εĻ�������*/
    {
        if(fabs(a-b)<=EPS&&fabs(b-c)<=EPS&&fabs(c-a)<=EPS)
        {
            printf("�ȱ�");
            flag=0;  /*�ñ�־����Ϊ0ֵ*/
        }
        else if(fabs(a-b)<=EPS||fabs(b-c)<=EPS||fabs(c-a)<=EPS)
        {
            printf("����");
            flag=0;  /*�ñ�־����Ϊ0ֵ*/
        }
        if(fabs(a*a+b*b-c*c)<=EPS||fabs(a*a+c*c-b*b)<=EPS||fabs(b*b+c*c-a*a)<=EPS)
        {
            printf("ֱ��");
            flag=0;  /*�ñ�־����Ϊ0ֵ*/
        }
        if(flag) /*��flagΪ��0������һ��������*/
        {
            printf("һ��");
        }
        printf("������\n");
    }
    else  /*���������*/
    {
        printf("����\n");
    }
    return 0;
}
