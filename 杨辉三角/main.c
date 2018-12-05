#include<stdio.h>
int main()
{
    int arr[25][25]={{0},{0}};
    int i,j,n;
    printf("Input n (n<=10):\n");
    scanf("%d", &n);
    for (i=1; i<n; i++)
    {
        arr[i-1][0]=1;
        for (j=0; j<=i; j++)
        {
            arr[i][j] =arr[i-1][j-1] + arr[i-1][j];
        }
    }
    for (i=0; i<n; i++)
    {
        for (j=0; j<=i; j++)
        {
            printf("%4d",arr[i][j]);
        }
        printf("\n");
    }
    return 0;
}
