#include <stdio.h>
int hour,minute,second;
void Updata()
{
    second++;
    if (second == 60)
    {
        second = 0;
        minute++;
    }
    if (minute == 60)
    {
        minute = 0;
        hour++;
    }
    if (hour == 24)
    {
        hour = 0;
    }
}
void Display()
{
    printf("%d:%d:%d\n",hour,minute,second);
}
void Delay()
{
    int t;
    for (t = 0;t < 1000000;t++);
}
int main()
{
    int i;
    hour = minute = second = 0;
    for (i = 0;i < 1000000;i++)
    {
        Updata();
        Display();
        Delay();
    }
    return 0;
}
