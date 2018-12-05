#include <stdio.h>
#include <stdlib.h>

#define MAX 10

void select_sort(int *p, int n);
void insert_sort(int *p, int n);
void bubble_sort(int *p, int n);
void shell_sort(int *p, int n);
void quick_sort(int *p, int low, int high);
void sift(int *p, int n, int s);
void heap_sort(int *p, int n);
void input(int a[]);
void output(int *p);
void merge(int a[], int start, int mid, int end);
void merge_sort(int a[], int start, int end);
void SequenceSearch(int *fp,int Length);
void Search(int *fp,int Length);
void Sort(int *fp,int Length);

int main()
{
    int start=0,end=3;
    int *p,i,a[MAX];
    int count=MAX;
    int arr[MAX];
    int choise=0;
    /*printf("������������ݵĸ���:\n");
    scanf("%d",&count);*/
    /* printf("������%d������\n",count);
    for(i=0;i<count;i++)
    {
        scanf("%d",&arr[i]);
    }*/
    /*¼���������*/
    input(a);
    printf("�����ʼ����Ϊ��\n");
    output(a);
    printf("\n");
    do
    {
        printf("1.ʹ��˳���ѯ.\n2.ʹ�ö��ֲ��ҷ�����.\n3.�˳�\n");
        scanf("%d",&choise);
        if(choise==1)
        SequenceSearch(a,count);
        else if(choise==2)
        Search(a,count);
        else if(choise==3)
        break;
    } while (choise==1||choise==2||choise==3);

    /*����ѡ������*/
    p = a;
    printf("ѡ������֮�������:\n");
    select_sort(p,MAX);
    output(a);
    printf("\n");

    /*����ֱ�Ӳ�������*/
    printf("ֱ�Ӳ�������֮�������:\n");
    p = a;
    insert_sort(p,MAX);
    output(a);
    printf("\n");

    /*����ð������*/
    printf("ð������֮�������:\n");
    p = a;
    insert_sort(p,MAX);
    output(a);
    printf("\n");

    /*���Կ�������*/
    printf("��������֮�������:\n");
    p = a;
    quick_sort(p,0,MAX-1);
    output(a);
    printf("\n");

    /*���Զ�����*/
    printf("������֮�������:\n");
    p = a;
    heap_sort(p,MAX);
    output(a);
    printf("\n");

    /*���Թ鲢����*/
    printf("�鲢����֮�������:\n");
    p = a;
    merge_sort(a,start,end);
    output(a);
    printf("\n");
    return 0;
}
/*-----------------------------------------*/
//ѡ�����������������ƣ������׵�ַ��������Ԫ�ظ�����������Ԫ�������ҳ���С��ֵ���Դ�ѭ��
void select_sort(int *p, int n)
{
    int i,j,min,t;
    for (i = 0; i < n-1; i++) //Ҫѡ��Ĵ���Ϊ0~n-2����n-1��
    {
        min = i;  //���赱ǰ�±�Ϊi������С���ȽϺ��ٵ���
        for (j = i + 1; j < n; j++)  //ѭ���ҳ���С�������±����ĸ�
        {
            //����
            if (*(p+j) < *(p+min))
                min = j;  //������������ǰ���С������������±�
        }
        if (min != i)  //���min��ѭ���иı��ˣ�����Ҫ�ı�����
        {
            t = *(p + i);
            *(p + i) = *(p + min);
            *(p + min) = t;
        }
    }
}
/*-----------------------------------------*/
//ֱ�Ӳ������򣬴Ӻ���������ǰ��һ���Ƚϣ���С�ķ���ǰ��
void insert_sort(int *p, int n)
{
    int i,j,t;
    for (i = 1; i < n; i++)
    {
        /*�ݴ��±�Ϊi������ע��:�±��1��ʼ,ԭ����ǿ�ʼʱ
        ��һ�������±�Ϊ0����,ǰ��û���κ���,����һ��,��Ϊ
        �����ź�˳��ġ�
        */
        t = *(p + i);  //�м���������ڽ���λ��
        /*ע��:j=i-1,j--,��������±�Ϊi����,����ǰ�����������Ҳ���λ�á�*/
        for (j = i - 1; j >= 0 && t < *(p + j); j--)
        {
            /*�����������������Ų��
            ����������t���±�Ϊ0������С,��Ҫ������ǰ��,j==-1,�˳�ѭ��*/
            *(p + i + j) = *(p + j);
        }
        /*�ҵ��±�Ϊi�����ķ���λ��*/
        *(p + j + 1) = t;
    }
}
/*-----------------------------------------*/
//ð����������������Ƚϣ��������ҳ���������󣬲���¼ÿ��ѭ������±�ֵ�����ٺ����ѭ������
void bubble_sort(int *p, int n)
{
    int j,k,h,t;
    for (h = n - 1; h > 0; h = k)   //ѭ����û�бȽϷ�Χ
    {
        for (j = 0, k = 0; j < h; j++)  //ÿ��Ԥ��k=0,ѭ��ɨ������k
        {
            if (*(p + j) > *(p + j + 1))  //��ķ��ں���,С�ķŵ�ǰ��
            {
                t = *(p + j);
                *(p + j) = *(p + j + 1);
                *(p + j + 1) = t;
                k = j;  //��������³���λ�á�����k����Ķ��������ź��˵ġ�
            }
        }
    }
}
/*-----------------------------------------*/
//ϣ������
void shell_sort(int *p, int n)
{
    int h,j,k,t;
    for (h = h / 2; h > 0; h = h / 2)  //��������
    {
        for (j = h; j < n; j++)  //���ʵ���Ͼ��������ֱ�Ӳ�������
        {
            t = *(p + j);
            for (k = j - h; k >= 0 && *(p + k); k -= h)
                *(p + k + h) = *(p + k);
            *(p + k + h) = t;
        }
    }
}
/*-----------------------------------------*/
/*�������������ַ��Ԫ����ֹ�±�*/
void quick_sort(int *p, int low, int high)
{
    int i,j,t;
    //Ҫ�����Ԫ����ֹ�±�,��֤С�ķ������,��ķ����ұߡ��������±�Ϊlow��Ԫ��Ϊ��׼��
    if (low < high)
    {
        i = low;
        j = high;
        t = *(p + low);  //�ݴ��׼�����
        while (i < j)  //ѭ��ɨ��
        {
            while (i < j && *(p + j) > t)  //���ұߵ�ֻҪ�Ȼ�׼����Է����ұ�
                j--;  //ǰ��һ��λ��
            if (i < j)
            {
                *(p + i) = *(p + j);  //�����ѭ���˳�:�����ֱȻ�׼��С����,�滻��׼�����
                i++;  //����һ��λ��,���Դ�Ϊ��׼��
            }
            while (i < j && *(p + i) <= t)  //����ߵ�ֻҪС�ڵ��ڻ�׼���Է������
                i++;  //����һ��λ��
            if (i < j)
            {
                *(p + j) = *(p + i);  //�����ѭ���˳�:�����ֱȻ�׼������,�ŵ��ұ�
                j--;  //ǰ��һ��λ��
            }
        }
        *(p + i) = t;  //һ��ɨ�����,�ŵ��ʵ�λ��
        quick_sort(p, low, i - 1);  //�Ի�׼����ߵ�����ִ�п�������
        quick_sort(p, i + 1, high);  //�Ի�׼���ұߵ�����ִ�п�������
    }
}
/*-----------------------------------------*/
//��͸����
void sift(int *p, int n, int s)
{
    int t,k,j;
    t = *(p + s);  //�ݴ濪ʼԪ��
    k = s;  //��ʼԪ���±�
    j = 2 * k + 1;  //������Ԫ���±�
    while (j < n)
    {
        if (j < n-1 && *(p+j) < *(p+j+1))  //���Ƿ�����ѵ�����:����ͼ�����һ�ֱȽ�,���������
            j++;
        if (t < *(p+j))  //����*
        {
            *(p+k) = *(p+j);
            k = j;  //������,��ʼԪ��Ҳ��֮����
            j = 2*k + 1;
        }
        else  //û����Ҫ������,�Ѿ��Ǹ�����,�˳�ѭ����
        {
            break;
        }
    }
    *(p+k) = t;  //��ʼԪ�طŵ�����ȷλ��
}
//������
void heap_sort(int *p, int n)
{
    int i, k,t;
    for (i = n/2-1; i >= 0; i--)
        sift(p,n,i);  //��ʼ����
    for (k = n-1; k >= 1; k--)
    {
        t = *(p+0);  //�Ѷ��ŵ����
        *(p+0) = *(p+k);
        *(p+k) = t;
        sift(p,k,0);  //ʣ�µ����ٽ���
    }
}

/*-------------------------------------*/
/*����������������*/
void input(int a[])
{
    int i;
    srand( (unsigned int)time(NULL) );
    for (i = 0; i < MAX; i++)
    {
        a[i] = rand() % 100;
    }
    printf("\n");
}
/*����������뺯����*/
/*void input(int *p)
{
     int i;
     printf("������ %d ������ :\n",MAX);
      for (i=0; i<MAX; i++)
      {
       scanf("%d",p++);
      }
      printf("\n");
}*/
/*�������������*/
void output(int *p)
{
    int i;
    for ( i=0; i<MAX; i++)
    {
        printf("%d ",*p++);
    }
}
/*-------------------------------------*/
//�鲢�����кϲ��㷨
void merge(int a[], int start, int mid, int end)
{
     int i,k,j, temp1[10], temp2[10];
     int n1, n2;
     n1 = mid - start + 1;
     n2 = end - mid;

     // ����ǰ�벿������
     for ( i = 0; i < n1; i++)
     {
         temp1[i] = a[start + i];
     }
     // ������벿������
     for (i = 0; i < n2; i++)
     {
         temp2[i] = a[mid + i + 1];
     }
     // �Ѻ����Ԫ�����õĺܴ�
     temp1[n1] = temp2[n2] = 1000;
     // ���ɨ������������Ȼ��ŵ���Ӧ��λ��ȥ
     for ( k = start, i = 0, j = 0; k <= end; k++)
     {
         if (temp1[i] <= temp2[j])
         {
             a[k] = temp1[i];
             i++;
         }
         else
         {
             a[k] = temp2[j];
             j++;
         }
     }
}
//�鲢����
void merge_sort(int a[], int start, int end)
{
    if (start < end)
    {
        int i = (end + start)/2;
        merge_sort(a, start, i);
        merge_sort(a, i+1, end);
        merge(a, start, i, end);
    }
}
/*-------------------------------------*/
//˳�����
void SequenceSearch(int *fp,int Length)
{
    int i;
    int data;
    printf("��ʼʹ��˳���ѯ.\n����������Ҫ���ҵ�����.\n");
    scanf("%d",&data);
    for(i=0;i<Length;i++)
    if(fp[i]==data)
    {
        printf("����%d�β���,���ҵ�����%d,����λ��Ϊ%d.\n",i+1,data,i);
        return ;
    }
    printf("����%d�β���,δ�ܲ��ҵ�����%d.\n",i,data);
}
/*���ֲ���*/
void Search(int *fp,int Length)
{
    int data;
    int bottom,top,middle;
    int i=0;
    printf("��ʼʹ�ö��ֲ�ѯ.\n����������Ҫ���ҵ�����.\n");
    scanf("%d",&data);
    printf("���ڶ��ֲ��ҷ�Ҫ�������������,���ڿ�ʼΪ��������.\n");
    Sort(fp,Length);
    printf("���������Ѿ��Ǵ�С��������,���潫��ʼ����.\n");
    bottom=0;
    top=Length;
    while (bottom<=top)
    {
        middle=(bottom+top)/2;
        i++;
        if(fp[middle]<data)
        {
            bottom=middle+1;
        }
        else if(fp[middle]>data)
        {
            top=middle-1;
        }
        else
        {
            printf("����%d�β���,���ҵ�����%d,�������ı��е�λ��Ϊ%d.\n",i,data,middle);
            return;
        }
    }
    printf("����%d�β���,δ�ܲ��ҵ�����%d.\n",i,data);
}

void Sort(int *fp,int Length)
{
    int temp;
    int i,j,k;
    printf("���ڿ�ʼΪ��������,���н�����Ǵ�С����.\n");
    for(i=0;i<Length;i++)
    for(j=0;j<Length-i-1;j++)
        if(fp[j]>fp[j+1])
        {
            temp=fp[j];
            fp[j]=fp[j+1];
            fp[j+1]=temp;
        }
        printf("�������!\n�����������������:\n");
        for(k=0;k<Length;k++)
        {
            printf("%5d",fp[k]);
        }
        printf("\n");
}
