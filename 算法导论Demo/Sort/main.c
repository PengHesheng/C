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
    /*printf("请输入你的数据的个数:\n");
    scanf("%d",&count);*/
    /* printf("请输入%d个数据\n",count);
    for(i=0;i<count;i++)
    {
        scanf("%d",&arr[i]);
    }*/
    /*录入测试数据*/
    input(a);
    printf("随机初始数组为：\n");
    output(a);
    printf("\n");
    do
    {
        printf("1.使用顺序查询.\n2.使用二分查找法查找.\n3.退出\n");
        scanf("%d",&choise);
        if(choise==1)
        SequenceSearch(a,count);
        else if(choise==2)
        Search(a,count);
        else if(choise==3)
        break;
    } while (choise==1||choise==2||choise==3);

    /*测试选择排序*/
    p = a;
    printf("选择排序之后的数据:\n");
    select_sort(p,MAX);
    output(a);
    printf("\n");

    /*测试直接插入排序*/
    printf("直接插入排序之后的数据:\n");
    p = a;
    insert_sort(p,MAX);
    output(a);
    printf("\n");

    /*测试冒泡排序*/
    printf("冒泡排序之后的数据:\n");
    p = a;
    insert_sort(p,MAX);
    output(a);
    printf("\n");

    /*测试快速排序*/
    printf("快速排序之后的数据:\n");
    p = a;
    quick_sort(p,0,MAX-1);
    output(a);
    printf("\n");

    /*测试堆排序*/
    printf("堆排序之后的数据:\n");
    p = a;
    heap_sort(p,MAX);
    output(a);
    printf("\n");

    /*测试归并排序*/
    printf("归并排序之后的数据:\n");
    p = a;
    merge_sort(a,start,end);
    output(a);
    printf("\n");
    return 0;
}
/*-----------------------------------------*/
//选择排序，输入数组名称（数组首地址）、数组元素个数，在所有元素中先找出最小的值，以此循环
void select_sort(int *p, int n)
{
    int i,j,min,t;
    for (i = 0; i < n-1; i++) //要选择的次数为0~n-2，共n-1次
    {
        min = i;  //假设当前下标为i的数最小，比较后再调整
        for (j = i + 1; j < n; j++)  //循环找出最小的数的下标是哪个
        {
            //升序
            if (*(p+j) < *(p+min))
                min = j;  //如果后面的数比前面的小，则记下它的下标
        }
        if (min != i)  //如果min在循环中改变了，就需要改变数据
        {
            t = *(p + i);
            *(p + i) = *(p + min);
            *(p + min) = t;
        }
    }
}
/*-----------------------------------------*/
//直接插入排序，从后面依次与前面一个比较，把小的放在前面
void insert_sort(int *p, int n)
{
    int i,j,t;
    for (i = 1; i < n; i++)
    {
        /*暂存下标为i的数，注意:下标从1开始,原因就是开始时
        第一个数即下标为0的数,前面没有任何数,单单一个,认为
        它是排好顺序的。
        */
        t = *(p + i);  //中间变量，用于交换位置
        /*注意:j=i-1,j--,这里就是下标为i的数,在它前面有序列中找插入位置。*/
        for (j = i - 1; j >= 0 && t < *(p + j); j--)
        {
            /*如果满足条件就往后挪。
            最坏的情况就是t比下标为0的数都小,它要放在最前面,j==-1,退出循环*/
            *(p + i + j) = *(p + j);
        }
        /*找到下标为i的数的放置位置*/
        *(p + j + 1) = t;
    }
}
/*-----------------------------------------*/
//冒泡排序，两个两个间比较，将最大的找出来放在最后，并记录每次循环后的下标值，减少后面的循环次数
void bubble_sort(int *p, int n)
{
    int j,k,h,t;
    for (h = n - 1; h > 0; h = k)   //循环到没有比较范围
    {
        for (j = 0, k = 0; j < h; j++)  //每次预置k=0,循环扫描后更新k
        {
            if (*(p + j) > *(p + j + 1))  //大的放在后面,小的放到前面
            {
                t = *(p + j);
                *(p + j) = *(p + j + 1);
                *(p + j + 1) = t;
                k = j;  //保存最后下沉的位置。这样k后面的都是排序排好了的。
            }
        }
    }
}
/*-----------------------------------------*/
//希尔排序
void shell_sort(int *p, int n)
{
    int h,j,k,t;
    for (h = h / 2; h > 0; h = h / 2)  //控制增量
    {
        for (j = h; j < n; j++)  //这个实际上就是上面的直接插入排序
        {
            t = *(p + j);
            for (k = j - h; k >= 0 && *(p + k); k -= h)
                *(p + k + h) = *(p + k);
            *(p + k + h) = t;
        }
    }
}
/*-----------------------------------------*/
/*快速排序输入地址和元素起止下标*/
void quick_sort(int *p, int low, int high)
{
    int i,j,t;
    //要排序的元素起止下标,保证小的放在左边,大的放在右边。这里以下标为low的元素为基准点
    if (low < high)
    {
        i = low;
        j = high;
        t = *(p + low);  //暂存基准点的数
        while (i < j)  //循环扫描
        {
            while (i < j && *(p + j) > t)  //在右边的只要比基准点大仍放在右边
                j--;  //前移一个位置
            if (i < j)
            {
                *(p + i) = *(p + j);  //上面的循环退出:即出现比基准点小的数,替换基准点的数
                i++;  //后移一个位置,并以此为基准点
            }
            while (i < j && *(p + i) <= t)  //在左边的只要小于等于基准点仍放在左边
                i++;  //后移一个位置
            if (i < j)
            {
                *(p + j) = *(p + i);  //上面的循环退出:即出现比基准点大的数,放到右边
                j--;  //前移一个位置
            }
        }
        *(p + i) = t;  //一遍扫描完后,放到适当位置
        quick_sort(p, low, i - 1);  //对基准点左边的数再执行快速排序
        quick_sort(p, i + 1, high);  //对基准点右边的数再执行快速排序
    }
}
/*-----------------------------------------*/
//渗透建堆
void sift(int *p, int n, int s)
{
    int t,k,j;
    t = *(p + s);  //暂存开始元素
    k = s;  //开始元素下标
    j = 2 * k + 1;  //右子树元素下标
    while (j < n)
    {
        if (j < n-1 && *(p+j) < *(p+j+1))  //断是否满足堆的条件:满足就继续下一轮比较,否则调整。
            j++;
        if (t < *(p+j))  //调整*
        {
            *(p+k) = *(p+j);
            k = j;  //调整后,开始元素也随之调整
            j = 2*k + 1;
        }
        else  //没有需要调整了,已经是个堆了,退出循环。
        {
            break;
        }
    }
    *(p+k) = t;  //开始元素放到它正确位置
}
//堆排序
void heap_sort(int *p, int n)
{
    int i, k,t;
    for (i = n/2-1; i >= 0; i--)
        sift(p,n,i);  //初始建堆
    for (k = n-1; k >= 1; k--)
    {
        t = *(p+0);  //堆顶放到最后
        *(p+0) = *(p+k);
        *(p+k) = t;
        sift(p,k,0);  //剩下的数再建堆
    }
}

/*-------------------------------------*/
/*构造随机输出函数类*/
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
/*构造键盘输入函数类*/
/*void input(int *p)
{
     int i;
     printf("请输入 %d 个数据 :\n",MAX);
      for (i=0; i<MAX; i++)
      {
       scanf("%d",p++);
      }
      printf("\n");
}*/
/*构造输出函数类*/
void output(int *p)
{
    int i;
    for ( i=0; i<MAX; i++)
    {
        printf("%d ",*p++);
    }
}
/*-------------------------------------*/
//归并排序中合并算法
void merge(int a[], int start, int mid, int end)
{
     int i,k,j, temp1[10], temp2[10];
     int n1, n2;
     n1 = mid - start + 1;
     n2 = end - mid;

     // 拷贝前半部分数组
     for ( i = 0; i < n1; i++)
     {
         temp1[i] = a[start + i];
     }
     // 拷贝后半部分数组
     for (i = 0; i < n2; i++)
     {
         temp2[i] = a[mid + i + 1];
     }
     // 把后面的元素设置的很大
     temp1[n1] = temp2[n2] = 1000;
     // 逐个扫描两部分数组然后放到相应的位置去
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
//归并排序
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
//顺序查找
void SequenceSearch(int *fp,int Length)
{
    int i;
    int data;
    printf("开始使用顺序查询.\n请输入你想要查找的数据.\n");
    scanf("%d",&data);
    for(i=0;i<Length;i++)
    if(fp[i]==data)
    {
        printf("经过%d次查找,查找到数据%d,表中位置为%d.\n",i+1,data,i);
        return ;
    }
    printf("经过%d次查找,未能查找到数据%d.\n",i,data);
}
/*二分查找*/
void Search(int *fp,int Length)
{
    int data;
    int bottom,top,middle;
    int i=0;
    printf("开始使用二分查询.\n请输入你想要查找的数据.\n");
    scanf("%d",&data);
    printf("由于二分查找法要求数据是有序的,现在开始为数组排序.\n");
    Sort(fp,Length);
    printf("数组现在已经是从小到大排列,下面将开始查找.\n");
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
            printf("经过%d次查找,查找到数据%d,在排序后的表中的位置为%d.\n",i,data,middle);
            return;
        }
    }
    printf("经过%d次查找,未能查找到数据%d.\n",i,data);
}

void Sort(int *fp,int Length)
{
    int temp;
    int i,j,k;
    printf("现在开始为数组排序,排列结果将是从小到大.\n");
    for(i=0;i<Length;i++)
    for(j=0;j<Length-i-1;j++)
        if(fp[j]>fp[j+1])
        {
            temp=fp[j];
            fp[j]=fp[j+1];
            fp[j+1]=temp;
        }
        printf("排序完成!\n下面输出排序后的数组:\n");
        for(k=0;k<Length;k++)
        {
            printf("%5d",fp[k]);
        }
        printf("\n");
}
