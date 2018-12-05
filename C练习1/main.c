#include<stdio.h>
#define NUM 100
#define MAX 999999

void Dijkstra(int n, int v, int *dist, int *prev, int c[NUM][NUM])
{
	int a,d,f;
    int i;
    int s[NUM];
    for( i=1; i<=n; ++i)
    {
        dist[i] = c[v][i];
        s[i] = 0;     // 最开始都未使用过
        if(dist[i] == MAX)
            prev[i] = 0;
        else
            prev[i] = v;
    }
    dist[v] = 0;
    s[v] = 1;
    // 依次将未放入S集合的结点中，取dist[]最小值的结点，放入集合S中，直到S集合包含V中所有顶点
    for(a=2; a<=n; ++a)
    {
        int tmp = MAX;
        int u = v;
        // 找出当前未使用的点j的dist[j]最小值
        for(d=1; d<=n; ++d)
            if((!s[d]) && dist[d]<tmp)
            {
                u = d;              // u保存当前邻接点中距离最小的点的号码
                tmp = dist[d];
            }
        s[u] = 1;    // 表示u点已存入S集合中

        // 更新路径
        for(f=1; f<=n; ++f)
            if((!s[f]) && c[u][f]<MAX)
            {
                int newdist = dist[u] + c[u][f];
                if(newdist < dist[f])
                {
                    dist[f] = newdist;
                    prev[f] = u;
                }
            }
    }
}

int main()
{
    FILE* fp=fopen("input.txt", "r");
    // 各数组都从下标1开始
    int dist[NUM];     // 当前点到源点的最短路径长度
    int prev[NUM];     // 当前点的前一个结点
    int c[NUM][NUM];   // 图的两点间路径长度
    int n=6,lon=10;             // 图的顶点数和路径数
    int i,j,k,l,m,z,dd;
    int p, q, len;          // 输入p, q两点及其路径长度
    for(i=1; i<=n; ++i)
        for(j=1; j<=n; ++j)
            c[i][j] = MAX;// 最初设两点间路径长度都为无穷大
 for(k=1; k<=lon; ++k)
 {
 	fscanf(fp,"%d",&p);
    fscanf(fp,"%d",&q);
 	fscanf(fp,"%d",&len);

        if(len < c[p][q])       // 有重边
        {
            c[p][q] = len;      // p指向q
            c[q][p] = len;      // q指向p，这样表示无向图
        }
    }
 fclose(fp);
 for(l=1; l<=n; ++l)
  dist[l] = MAX;
  printf("该图可化为矩阵：\n");
  printf("\n");
    for(m=1; m<=n; ++m)
    {
        for(z=1; z<=n; ++z)
            printf("%8d", c[m][z]);
        printf("\n\n");
    }
    Dijkstra(n, 1, dist, prev, c);
    // 最短路径长度
     for(i=1; i<=n; ++i)
     {
         printf("\n从顶点%d到顶点%d的最短路径为%d\n",1,i,dist[i]);
     }
    printf("\n");
    //建立并显示顶点1的路由转发表
    printf("\n顶点%d的路由转发表：\n",1);
    for(i=1;i<=n;i++)
    {
        printf("%d %d", i, prev[i]);
    }
    printf("\n");
    printf("目的地节点   下一跳节点   到目的节点距离\n");
    for(i=1; i<=n; i++)
    {
        //用k做中间变量，i节点的前驱
        k = prev[i];
        //当为顶点时或者顶点的相邻节点时，直接输出
        if (i == 1 || i == 2)
        {
            printf("     %d           %d               %d        \n",i, i,dist[i]);
        } else {
            //遍历所有的节点，寻找前驱
            for (j = 1; j<= n; j++)
            {
                //如果j节点等于i节点的前驱k
                if (j == k)
                {
                    //如果将节点就是顶点1，结束循环，i节点的下一跳就是k，
                    if (prev[j] == 1)
                    {
                        break;
                    }
                    //否则迭代前驱k，重新遍历，寻找与前驱相同的节点
                    k = prev[j];
                    j = 1;
                }
            }
            //输出i节点的后继
            printf("     %d           %d               %d        \n",i, k,dist[i]);
        }
    }
 return 0;
}
