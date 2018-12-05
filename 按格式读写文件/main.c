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
    DATE birthday;
    int score[4];
    float aver;
}STUDENT;
//从文件读出
void ReadFromFile(STUDENT stu[], int *n, int *m);
void Print(STUDENT stu[], int n, int m);
int main()
{
    STUDENT stu[N];
    int n,m=4;
    ReadFromFile(stu,&n,&m);
    Print(stu,n,m);
    return 0;
}
void ReadFromFile(STUDENT stu[], int *n, int *m)
{
    FILE *fp;
    int i,j;
    fp=fopen("F:\\C\\score.txt","r");
    if (fp == NULL)
    {
        printf("Failure\n");
        exit(0);
    }
    fscanf(fp,"%d\t%d\n",n,m);
    for (i=0; i<*n; i++)
    {
        fscanf(fp,"%10ld", &stu[i].sID);
        fscanf(fp,"%8s", stu[i].sName);
        fscanf(fp," %c", &stu[i].sSex);
        fscanf(fp,"%6d.%2d.%2d", &stu[i].birthday.year, &stu[i].birthday.month, &stu[i].birthday.day);
        for (j=0; j<*m; j++)
            fscanf(fp,"%4d", &stu[i].score[j]);
        fscanf(fp,"%f", &stu[i].aver);
    }
    fclose(fp);
}
void Print(STUDENT stu[], int n, int m)
{
    int i,j;
    for (i=0; i<n; i++)
    {
        printf("%10ld%8s%3c%6d.%02d.%02d",stu[i].sID,stu[i].sName,stu[i].sSex,stu[i].birthday.year,stu[i].birthday.month,stu[i].birthday.day);
        for (j=0; j<m; j++)
            printf("%4d",stu[i].score[j]);
        printf("%6.1f\n",stu[i].aver);
    }
}
//写入文件
/*void InputScore(STUDENT stu[], int n, int m);
void AverScore(STUDENT stu[], int n, int m);
void WriteToFile(STUDENT stu[], int n, int m);
int main()
{
    STUDENT stu[N];
    int n;
    printf("How many?");
    scanf("%d", &n);
    InputScore(stu,n,4);
    AverScore(stu,n,4);
    WriteToFile(stu,n,4);
    return 0;
}
//键入学生学号姓名性别出生年月和各科成绩
void InputScore(STUDENT stu[], int n, int m)
{
    int i,j;
    for (i=0; i<n; i++)
    {
        printf("Input recored %d:\n",i+1);
        scanf("%ld", &stu[i].sID);
        scanf("%s", stu[i].sName);
        scanf(" %c", &stu[i].sSex);
        scanf("%d", &stu[i].birthday.year);
        scanf("%d", &stu[i].birthday.month);
        scanf("%d", &stu[i].birthday.day);
        for (j=0; j<m; j++)
            scanf("%d", &stu[i].score[j]);
    }
}
//计算平均分，存入Aver
void AverScore(STUDENT stu[], int n, int m)
{
    int i,j,sum;
    for (i=0; i<n; i++)
    {
        sum=0;
        for (j=0; j<m; j++)
            sum += stu[i].score[j];
        stu[i].aver = (float)sum/m;
    }
}
void WriteToFile(STUDENT stu[], int n, int m)
{
    FILE *fp;
    int i,j;
    fp=fopen("F:\\C\\score.txt","w");
    if (fp == NULL)
    {
        printf("Failure\n");
        exit(0);
    }
    fprintf(fp,"%d\t%d\n",n,m); //学生人数和课程门数
    for (i=0; i<n; i++)
    {
        fprintf(fp,"%10ld%8s%3c%6d.%02d.%02d",stu[i].sID,stu[i].sName,stu[i].sSex,stu[i].birthday.year,stu[i].birthday.month,stu[i].birthday.day);
        for (j=0; j<m; j++)
            fprintf(fp,"%4d",stu[i].score[j]);
        fprintf(fp,"%6.1f\n",stu[i].aver);
    }
    fclose(fp);
}*/
