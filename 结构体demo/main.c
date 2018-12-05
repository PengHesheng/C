#include <stdio.h>
#include <stdlib.h>
#define N 30
typedef struct date
{
    int year,month,day;
}DATE;
typedef struct student
    {
        long sID;
        char sName[10];
        char sSex;
        DATE sYearbirth;
        int score[4];
    }STUDENT;
/*void Func(struct date *pt)
{
    pt->year = 2000;
    pt->month = 5;
    pt->day = 12;
}*/
void InputScore(STUDENT stu[N], int n, int m)
{
    int i,j;
    for (i=0; i<n; i++)
    {
        printf("Input the record %d:\n",i+1);
        scanf("%ld", &stu[i].sID);
        scanf("%s",stu[i].sName);
        scanf(" %c", &stu[i].sSex);
        scanf("%d", &stu[i].sYearbirth.year);
        scanf("%d", &stu[i].sYearbirth.month);
        scanf("%d", &stu[i].sYearbirth.day);
        for (j=0; j<m; j++)
            scanf("%d", &stu[i].score[j]);
    }
}
void AverScore(STUDENT stu[], float aver[], int n, int m)
{
    int i,j,sum[N];
    for (i=0; i<n; i++)
    {
        sum[i]=0;
        for (j=0; j<m; j++)
            sum[i] = sum[i] + stu[i].score[j];
        aver[i] = (float)sum[i]/m;
    }
}
void Print(STUDENT s1[], float aver[], int n, int m)
{
    int i,j;
    printf("Results :\n");
    for (i=0; i<n; i++)
    {
        printf("%8ld%10s%2c\t%6d.%02d.%02d\t", s1[i].sID, s1[i].sName, s1[i].sSex, s1[i].sYearbirth.year, s1[i].sYearbirth.month, s1[i].sYearbirth.day);
        for (j=0; j<m; j++)
            printf("%4d",s1[i].score[j]);
        printf("%6.1f\n",aver[i]);
    }
}
int main()
{
    /*int i;
    STUDENT s1={2016210343,"PHS",'M',{1998,10,2},{80,80,90,91}};
    STUDENT s2={2016210342,"SJU",'W',{1998,01,02},{81,82,83,91}};
    STUDENT *a=&s1.sYearbirth;
    printf("%ld%10s%2c\t%6d.%02d.%02d\t", s1.sID, s1.sName, s1.sSex, s1.sYearbirth.year, s1.sYearbirth.month, s1.sYearbirth.day);
    for (i=0; i<4; i++)
        printf("%3d",s1.score[i]);
    printf("\n");
    Func(a);
    printf("%6d.%02d.%02d\t", s1.sYearbirth.year, s1.sYearbirth.month, s1.sYearbirth.day);
    printf("\nInput:\n");
    scanf("%ld", &s2.sID);
    scanf("%s", s2.sName);
    scanf(" %c", &s2.sSex);
    scanf("%d", &s2.sYearbirth.year);
    scanf("%d", &s2.sYearbirth.month);
    scanf("%d", &s2.sYearbirth.day);
    for (i=0; i<4; i++)
        scanf("%d", &s2.score[i]);
    printf("%ld%10s%2c\t%6d.%02d.%02d\t", s2.sID, s2.sName, s2.sSex, s2.sYearbirth.year, s2.sYearbirth.month, s2.sYearbirth.day);
    for (i=0; i<4; i++)
        printf("%3d", s2.score[i]);*/

    float aver[N];
    STUDENT s[N];
    int n,m;
    printf("ow many students and classes?");
    scanf("%d %d", &n, &m);
    InputScore(s,n,m);
    AverScore(s,aver,n,m);
    Print(s,aver,n,m);
    return 0;
}
