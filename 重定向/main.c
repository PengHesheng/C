#include <stdio.h>
#include <stdlib.h>
int main()
{
    int i;
    if (freopen("F:\\C\\output.txt","w",stdout) == NULL) //如果能够重定向，返回文件指针，否则返回NULL
        fprintf(stderr,"error stdout");
    for (i=0; i<10; i++)
        printf("%3d",i);  //输出到文件中
    printf("kjl\n");
    fclose(stdout);
    return 0;
}
