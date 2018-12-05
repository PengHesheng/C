long Fact(int n)
{
    int i;
    long result=1;
    for(i=2;i<=n;i++)
    {
        result*=i;
    }
    //return n*Fact(-1);
    //return Fact(n-1,n*a)另外两种表示
    return result;
}   //阶乘


//利用数组判断某年某月的天数
#include <stdio.h>
int main()
{
    int days[2][12] = {{31,28,31,30,31,30,31,31,30,31,30,31},{31,29,31,30,31,30,31,31,30,31,30,31}};
    int year,month;
    do{
        printf("Input year,month:\n");
        scanf("%d.%d", &year, &month);
    }while(month < 1 || month > 12);   //对不合法输入的处理
    if (((year % 4 == 0) && (year % 100 != 0)) || year % 400 == 0)
    {
        printf("The days is :%d\n",days[1][month - 1]);
    }
    else
    {
        printf("The days is %d\n",days[1][month - 1]);
    }
    return 0;
}

//对数组进行交换法排序
void DataSort(int score[],int n)
{
    int i,j,temp;
    for (i = 0; i < n - 1; i++)
    {
        for (j = i + 1; j < n; j++)
        {
            if (score[j] > score[i])  //进行排序
            {
                temp = score[j];
                score[j] = score[i];
                score[i] = temp;   // 实现数组的交换，引入temp，防止数据丢失
            }
        }
    }
}

//对数组进行选择法排序
void DataSort(int score[],int n)
{
    int i,j,k,temp;
    for (i = 0; i < n - 1; i++)
    {
        k = i;
        for (j = i + 1; j < n; j++)
        {
            if (score[j] > score[k])  //进行排序
            {
                k = j; //记录最大数下标值
            }
        }
        if (k != i)   //若最大值得下标位置不在下标位置
        {
            temp = score[k];
            score[k] = score[i];
            score[i] = temp;
        }
    }
}

//线性查找数组元素
int LinSearch(long num[], long x, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (num[i] == x)
            return i;
    }
    return -1;
}

//拆半查找法
int BinSearch(int long num[], long x,int n)
{
    int low = 0,high = n - 1,mid;  //区间左端点low置为0，右端点置为n-1
    while (low <= high)            //若左小于右，则继续查找
    {
        mid = (high + low) / 2;    //取区间的中点
        if (x > num[mid])
            low = mid + 1;         //若x>num[mid]，则修改左
        else if (x < num[mid])
            high = mid + 1;        //若x<num[mid],则修改右
        else
            return mid;            //若找到，则返回下标值mid
    }
    return -1;
}

//求两个数的最大公约数 欧几里得算法
int MaxCommonFactor(int a,int b)
{
    int r;
    if (a <= 0 || b <= 0)
        return -1;
    do{
        r = a % b;
        a = b;
        b = r;
    }while(r != 0);
    return a;
}

//利用extern 声明在其他文件的函数，以此调用函数

int partition(int arr[], int low, int high)
{
    int key;
    key = arr[low];
    while(low < high)
    {
        while(low < high && arr[high] >= key)
            high--;
            if (low < high)
                arr[low++] = arr[high];
        while(low < high && arr[low] <= key)
            low++;
        if (low < high)
            arr[high--] = arr[low];
    }
    arr[low] = key;
    return low;
}

//快速排序
void quick_sort(int arr[], int start, int end)
{
    int pos;
    if (start < end)
    {
        pos = partition(arr,start,end);
        quick_sort(arr,start,pos-1);
        quick_sort(arr,pos+1,end);
    }
    return;
}
