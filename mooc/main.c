#include <stdio.h>
void f(int *i)
{
    *i = 100;
}
int main()
{
    int i = 9;
    f(&i);
    printf("%d\n",i);
    return 0;
}
