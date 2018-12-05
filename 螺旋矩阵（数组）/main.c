#include <stdio.h>
#define N 10
//按圈数控制循环,逆螺旋
void SetArray1(int a[][N], int len, int n)
{
    int m, k, level;
    len = n*n;
    level = n>0 ? (n+1)/2 : -1;
    for (m=0; m<level; m++)
    {
        for (k=m; k<n-m; k++)
            a[m][k] = len--;
        for (k=m+1; k <n-m-1; k++)
            a[k][n-m-1] = len--;
        for (k=n-m-1; k>m; k--)
            a[n-m-1][k] = len--;
        for (k=n-m-1; k>m; k--)
            a[k][m] = len--;
    }
}
//按列输入数组
void SetArray2(int a[][N],int len, int n)
{
    int i,j;
    for (i=0; i<n; i++)
    {
        for (j=0; j<n; j++)
            a[j][i] = len++;
    }
}
//按格数控制循环
void SetArray3(int a[][N],int len, int n)
{
    int start=0, border=n-1, k, m=1;
    while (m <= n*n)
    {
        if (start > border) return;
        else if (start == border)
        {
            a[start][start] = len;
            return;
        }
        else
        {
             for (k=start; k<=border-1; k++)
             {
                 a[start][k] = len++;
                 m++;
             }
             for (k=start; k<=border-1; k++)
             {
                 a[k][border] = len++;
                 m++;
             }
             for (k=border; k>=start+1; k--)
             {
                 a[border][k] = len++;
                 m++;
             }
             for (k=border; k>=start+1; k--)
             {
                 a[k][start] = len++;
                 m++;
             }
             start++;
             border--;

        }
    }
}
//顺螺旋
void SetArray4(int a[][N], int len, int n)
{
    int m, k, level;
    level = n>0 ? (n+1)/2 : -1;
    for (m=0; m<level; m++)
    {
        for (k=m; k<n-m; k++)
            a[m][k] = len++;
        for (k=m+1; k <n-m-1; k++)
            a[k][n-m-1] = len++;
        for (k=n-m-1; k>m; k--)
            a[n-m-1][k] = len++;
        for (k=n-m-1; k>m; k--)
            a[k][m] = len++;
    }
}

}
//输出数组
void PrintArray(int a[][N], int m, int n)
{
    int i,j;
    for (i=0; i<m; i++)
    {
        for (j=0; j<n; j++)
        {
            printf("%3d",a[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("\n");
}
int main()
{
    int n;
    int a1[N][N],a2[N][N],a3[N][N],a4[N][N]={0};
    scanf("%d", &n);
    SetArray1(a1,1,n);
    SetArray2(a2,1,n);
    SetArray3(a3,1,n);
    SetArray(a4,1,n);
    PrintArray(a1,n,n);
    PrintArray(a2,n,n);
    PrintArray(a3,n,n);
    PrintArray(a4,n,n);
    return 0;
}
