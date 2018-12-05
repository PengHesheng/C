#include <stdio.h>
#define N 30
int Read(int score[])
{
    int i=-1;
    do{
        i++;
        scanf("%d",&score[i]);
    }while(score[i] > 0);
    return i;
}
void Print(int score[], int n)
{
    int i;
    for (i=0; i<n; i++)
    {
        printf("%3d",score[i]);
    }
    printf("\n");
}
/*void Ascend(int a[], int n)//����
{
    int i,j,k;
    for (i=0; i<n; i++)
    {
        k=i;
        for (j=i+1; j<n; j++)
        {
            if (a[j] < a[k])
                k=j;
        }
        if (k != i)
            Swap(&a[k],&a[i]);
    }
}
void Descend(int a[], int n)//����
{
    int i,j,k;
    for (i=0; i<n; i++)
    {
        k=i;
        for (j=i+1; j<n; j++)
        {
            if (a[j] > a[k])
                k=j;
        }
        if (k != i)
            Swap(&a[k],&a[i]);
    }
}*/
//���������������ļ̳У��򻯴���
int Selection(int a[], int n, int (*compare)(int a, int b))
{
    int i,j,k;
    for (i=0; i<n; i++)
    {
        k=i;
        for (j=i+1; j<n; j++)
        {
            if ((*compare)(a[j],a[k]))//���ú���ָ��compare
                k=j;
        }
        if (k != i)
            Swap(&a[k],&a[i]);
    }
}
int Ascend(int a, int b)
{
    return a<b;
}
int Descend(int a, int b)
{
    return a>b;
}
int Swap(int *a, int *b)
{
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
int main()
{
    int score[N],n;
    n=Read(score);
    printf("%d\n",n);
    Print(score,n);
    Selection(score,n,Ascend);
    Print(score,n);
    Selection(score,n,Descend);
    Print(score,n);
    return 0;
}
/*#include <stdio.h>
#include <string.h>
void print_reverse(char *s)
{
    size_t len = strlen(s);
    char *t = s + len - 1;
    while (t >= s)
    {
        printf("%c",*t);
        t=t-1;
    }
    puts("");
}

int main()
{
    char *juices[] = {
        "dragonfruit", "waterberry", "sharonfruit", "uglifruit",
        "rumberry", "kiwifruit", "mulberry", "strawberry",
        "blueberry", "blackberry", "starfruit"  };
    char *a;
    puts(juices[6]);
    print_reverse(juices[7]);
    a = juices[2];
    juices[2] = juices[8];
    juices[8] = a;
    puts(juices[8]);
    print_reverse(juices[(18 + 7) / 5]);
    puts(juices[2]);
    print_reverse(juices[9]);
    juices[1] = juices[3];
    puts(juices[10]);
    print_reverse(juices[1]);
    return 0;
}*/

//����ָ���ַ
/*
#include <stdio.h>
void swap(int **p1, int **p2)
{
    int *p;
    p=*p1;
    *p1=*p2;
    *p2=p;
    printf("1=%p,2=%p\n",p1,p2);
    printf("max=%d,min=%d\n",**p1,**p2);
}
int main()
{
    int a,b;
    int *pointer_1, *pointer_2;
    int **x, **y;
    x=&pointer_1;
    y=&pointer_2;
    printf("Input two numbers :");
    scanf("%d %d", &a, &b);
    pointer_1=&a;
    pointer_2=&b;
    printf("1=%p,2=%p\n",pointer_1,pointer_2);
    if (a<b)
        swap(x,y);
    printf("1=%p,2=%p\n",pointer_1,pointer_2);
    printf("max=%d,min=%d\n",*pointer_1,*pointer_2);
    printf("%d %3d",a,b);
    return 0;
}*/
//����ָ�룬����һ��ָ��ĵ�ַ�������ı�һ��ָ����ָ��Ķ���a��b�����ֵ��û�з�������
//�뽻����ֵ���ƣ�ֻ�н�����ַ�����ܸı�
