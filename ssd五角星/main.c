#include <stdio.h>
#include <stdlib.h>
int main()
{
    int n1,n2,n3,n4,j1,j2,j3,j4,k1,k2,k3,k4,a4,b4;
    system("color 0e");
    for(n1=1;n1<6;n1++)
    {
        for(j1=1;j1<19-n1;j1++)
            printf(" ");
        for(k1=1;k1<=2*n1-1;k1++)
            printf("*");
            printf("\n");
    }
    for(n2=1;n2<=5;n2++)
    {
        for(j2=1;j2<3*n2-3;j2++)
            printf(" ");
        for(k2=1;k2<=42-6*n2;k2++)
            printf("*");
            printf("\n");
    }
    for(n3=1;n3<3;n3++)
    {
        for(j3=1;j3<12-n3;j3++)
            printf(" ");
        for(k3=1;k3<=12+2*n3;k3++)
            printf("*");
            printf("\n");
    }
    for(n4=1;n4<5;n4++)
    {
        for(j4=1;j4<10-n4;j4++)
            printf(" ");
        for(k4=1;k4<=10-2*n4;k4++)
            printf("*");
        for(a4=1;a4<6*n4-3;a4++)
            printf(" ");
        for(b4=1;b4<10-2*n4;b4++)
            printf("*");
            printf("\n");
    }
    return 0;
}
