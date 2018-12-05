#include <stdio.h>
int main()
{
    int i;
    double a,pi,t;
    t = 0;
    for (i = 1; i <= 9999999; i++)
    {
        a = (1.0/(4 * i - 3)) - (1.0/(4 * i - 1));
        t = a + t;
    }
    pi = 4 * t;
    printf("%.5lf",pi);
    return 0;
}
