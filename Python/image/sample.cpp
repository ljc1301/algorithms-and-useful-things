#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
using namespace std;
const int maxn=2005;
const int maxlen=1005;
const int maxtupe=5;
int mat[maxn][maxn][maxtupe],n=-1,m=-1,tupe=3,t,cnt;
char s[maxlen],temp[maxlen];
FILE *f;
int main()
{
    int i,j,k;
    memset(mat,0,sizeof(mat));
    while(scanf("%s",s)==1)
    {
        sprintf(temp,"python image/load.py image/%s > test.in",s);
        if(system(temp)) break;
        f=fopen("test.in","r");
        fscanf(f,"%d",&t);
        if(n==-1) n=t;
        else if(n!=t) { fclose(f); break; }
        fscanf(f,"%d",&t);
        if(m==-1) m=t;
        else if(m!=t) { fclose(f); break; }
        fscanf(f,"%d",&t);
        if(tupe==-1) tupe=t;
        else if(tupe!=t) { fclose(f); break; }
        for(i=0;i<n;i++)
            for(j=0;j<m;j++)
                for(k=0;k<tupe;k++)
                {
                    fscanf(f,"%d",&t);
                    mat[i][j][k]+=t;
                }
        fclose(f);
        system("echo > test.in");
        cnt++;
    }
    f=fopen("test.out","w");
    fprintf(f,"%d %d\nRGB\n",n,m);
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            for(k=0;k<tupe;k++)
            {
                if(k)
                    fprintf(f," ");
                fprintf(f,"%d",(mat[i][j][k]+(cnt>>1))/cnt);
            }
            fprintf(f,"\n");
        }
        fprintf(f,"\n");
    }
    fclose(f);
    system("python image/save.py image/out.jpg < test.out");
    system("echo > test.out");
    return 0;
}
