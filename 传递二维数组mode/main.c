#include <stdio.h>
#define STUD_N 40
#define COURSE_N 3
void ReadScore(int score[][COURSE_N],long num[],int n)
{
    int i,j;
    printf("Input ID and score as:MT EN PH:\n");
    for (i = 0; i < n;i++)
    {
        scanf("%ld", &num[i]);
        for (j = 0; j < COURSE_N; j++)
        {
            scanf("%d", &score[i][j]);
        }
    }
}

void AverforStud(int score[][COURSE_N],int sum[],float aver[],int n)
{
    int i,j;
    for (i = 0;i < n;i++)
    {
        sum[i] = 0;
        for (j = 0; j < COURSE_N; j++)
        {
            sum[i] = sum[i] + score[i][j];   //�����ܷ�
        }
        aver[i] = (float)sum[i] / COURSE_N;  //�����i�����ܷ�
    }
}

void AverforCourse(int score[][COURSE_N],int sum[],float aver[],int n)
{
    int i,j;
    for (j = 0; j < COURSE_N;j++)
    {
        sum[j] = 0;
        for (i = 0;i < n; i++)
        {
            sum[j] = sum[j] + score[i][j];  //����ÿγ̵��ܷ�
        }
        aver[j] = (float)sum[j] / n;     //����ÿγ̵�ƽ����
    }
}

void Print(int score[][COURSE_N],long num[],int sumS[],float averS[],int sumC[],float averC[],int n)
{
    int i,j;
    printf("Student's ID\t MT\t EN\t PH\t SUM\t AVER\n");
    for (i = 0; i < n; i++)
    {
        printf("%12ld\t",num[i]);     //��ӡѧ��
        for (j = 0; j < COURSE_N; j++)
        {
            printf("%4d\t",score[i][j]);   //��ӡÿ�ſγɼ�
        }
        printf("%4d\t%5.1f\n",sumS[i],averS[i]);   //��ӡ�ֺܷ�ƽ����
    }
    printf("SumofCouse\t");
    for (j = 0; j < COURSE_N; j++)   //��ӡÿ�ſε��ܷ�
    {
        printf("%4d\t",sumC[j]);
    }
    printf("\nAverofCourse\t");
    for (j = 0; j < COURSE_N; j++)   //��ӡÿ�ſε�ƽ����
    {
        printf("%4.1f\t",averC[j]);
    }
    printf("\n");
}

int main()
{
    int score[STUD_N][COURSE_N],sumS[STUD_N],sumC[COURSE_N],n;
    long num[STUD_N];
    float averS[STUD_N],averC[COURSE_N];
    printf("input the total number of the students(n <= 40):");
    scanf("%d", &n);
    ReadScore(score,num,n);  //����ɼ�ѧ�ţ���������
    AverforStud(score,sumS,averS,n);  //����ÿ��ѧ����ƽ����
    AverforCourse(score,sumC,averC,n);  //����ÿ�ſε�ƽ����
    Print(score,num,sumS,averS,sumC,averC,n);
    return 0;
}
