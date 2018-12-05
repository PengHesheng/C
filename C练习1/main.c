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
        s[i] = 0;     // �ʼ��δʹ�ù�
        if(dist[i] == MAX)
            prev[i] = 0;
        else
            prev[i] = v;
    }
    dist[v] = 0;
    s[v] = 1;
    // ���ν�δ����S���ϵĽ���У�ȡdist[]��Сֵ�Ľ�㣬���뼯��S�У�ֱ��S���ϰ���V�����ж���
    for(a=2; a<=n; ++a)
    {
        int tmp = MAX;
        int u = v;
        // �ҳ���ǰδʹ�õĵ�j��dist[j]��Сֵ
        for(d=1; d<=n; ++d)
            if((!s[d]) && dist[d]<tmp)
            {
                u = d;              // u���浱ǰ�ڽӵ��о�����С�ĵ�ĺ���
                tmp = dist[d];
            }
        s[u] = 1;    // ��ʾu���Ѵ���S������

        // ����·��
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
    // �����鶼���±�1��ʼ
    int dist[NUM];     // ��ǰ�㵽Դ������·������
    int prev[NUM];     // ��ǰ���ǰһ�����
    int c[NUM][NUM];   // ͼ�������·������
    int n=6,lon=10;             // ͼ�Ķ�������·����
    int i,j,k,l,m,z,dd;
    int p, q, len;          // ����p, q���㼰��·������
    for(i=1; i<=n; ++i)
        for(j=1; j<=n; ++j)
            c[i][j] = MAX;// ����������·�����ȶ�Ϊ�����
 for(k=1; k<=lon; ++k)
 {
 	fscanf(fp,"%d",&p);
    fscanf(fp,"%d",&q);
 	fscanf(fp,"%d",&len);

        if(len < c[p][q])       // ���ر�
        {
            c[p][q] = len;      // pָ��q
            c[q][p] = len;      // qָ��p��������ʾ����ͼ
        }
    }
 fclose(fp);
 for(l=1; l<=n; ++l)
  dist[l] = MAX;
  printf("��ͼ�ɻ�Ϊ����\n");
  printf("\n");
    for(m=1; m<=n; ++m)
    {
        for(z=1; z<=n; ++z)
            printf("%8d", c[m][z]);
        printf("\n\n");
    }
    Dijkstra(n, 1, dist, prev, c);
    // ���·������
     for(i=1; i<=n; ++i)
     {
         printf("\n�Ӷ���%d������%d�����·��Ϊ%d\n",1,i,dist[i]);
     }
    printf("\n");
    //��������ʾ����1��·��ת����
    printf("\n����%d��·��ת����\n",1);
    for(i=1;i<=n;i++)
    {
        printf("%d %d", i, prev[i]);
    }
    printf("\n");
    printf("Ŀ�ĵؽڵ�   ��һ���ڵ�   ��Ŀ�Ľڵ����\n");
    for(i=1; i<=n; i++)
    {
        //��k���м������i�ڵ��ǰ��
        k = prev[i];
        //��Ϊ����ʱ���߶�������ڽڵ�ʱ��ֱ�����
        if (i == 1 || i == 2)
        {
            printf("     %d           %d               %d        \n",i, i,dist[i]);
        } else {
            //�������еĽڵ㣬Ѱ��ǰ��
            for (j = 1; j<= n; j++)
            {
                //���j�ڵ����i�ڵ��ǰ��k
                if (j == k)
                {
                    //������ڵ���Ƕ���1������ѭ����i�ڵ����һ������k��
                    if (prev[j] == 1)
                    {
                        break;
                    }
                    //�������ǰ��k�����±�����Ѱ����ǰ����ͬ�Ľڵ�
                    k = prev[j];
                    j = 1;
                }
            }
            //���i�ڵ�ĺ��
            printf("     %d           %d               %d        \n",i, k,dist[i]);
        }
    }
 return 0;
}
