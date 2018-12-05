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
void InputScore(STUDENT stu[], int n, int m);
void AverScore(STUDENT stu[], int n, int m);
void WriteToFile(STUDENT stu[], int n);
int ReadFromFile(STUDENT stu[]);
void Print(STUDENT stu[], int n, int m);
void SearchInFile(char fileName[], long k);
int main()
{
    STUDENT stu[N];
    int n,m=4;
    printf("How many?");
    scanf("%d", &n);
    InputScore(stu,n,m);
    AverScore(stu,n,m);
    WriteToFile(stu,n);
    n=ReadFromFile(stu);
    Print(stu,n,m);
    //查找文件，文件的随机读写
    /*long k;
    scanf("%ld", &k);
    SearchInFile("F:\\C\\student.txt",k);*/
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
void WriteToFile(STUDENT stu[], int n)
{
    FILE *fp;
    fp=fopen("F:\\C\\student.txt","w");
    if (fp==NULL)
    {
        printf("Failure\n");
        exit(0);
    }
    fwrite(stu,sizeof(STUDENT),n,fp); //按数据块写文件,n表示数据块的个数
    fclose(fp);
}
int ReadFromFile(STUDENT stu[])
{
    FILE *fp;
    int i;
    fp=fopen("F:\\C\\student.txt","r");
    if (fp==NULL)
    {
        printf("Failure\n");
        exit(0);
    }
    for (i=0; !feof(fp); i++)
        fread(&stu[i],sizeof(STUDENT),1,fp);  //按数据块读文件
    fclose(fp);
    printf("Total students %d.\n",i-1);
    return i-1;
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
//查找数据
void SearchInFile(char fileName[], long k)
{
    FILE *fp;
    int j;
    STUDENT stu;
    fp=fopen(fileName,"r");
    if (fp==NULL)
    {
        printf("Failure\n");
        exit(0);
    }
    fseek(fp,(k-1)*sizeof(STUDENT),SEEK_SET);
    fread(&stu,sizeof(STUDENT),1,fp);
    printf("%10ld%8s%3c%6d.%02d.%02d",stu.sID,stu.sName,stu.sSex,stu.birthday.year,stu.birthday.month,stu.birthday.day);
    for (j=0; j<4; j++)
        printf("%4d",stu.score[j]);
    printf("%6.1f\n",stu.aver);
    fclose(fp);
}
