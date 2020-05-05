#include <bits/stdc++.h>
using namespace std;
#define r(n)(rand()%n)
unsigned char RD(int i,int j);
unsigned char BL(int i,int j);
unsigned char GR(int i,int j);

const int sz=50;
const int n=sz*3; // height
const int m=sz*3; // width
double u[n][m],t[n][m];
bool vis[n][m],locked[n][m];
inline void init()
{
    int i,j,_;
    double tot=1e99;
    for(i=0;i<n;i++)
        for(j=0;j<m;j++)
        {
            // vis[i][j]=((i/sz==0 && j/sz>=1) || (i/sz==1 && j/sz<3));
            // vis[i][j]=(i/sz!=2 || j/sz!=2);
            // vis[i][j]=((i-1.5*sz)*(i-1.5*sz)+(j-1.5*sz)*(j-1.5*sz))>=sz*sz;
            vis[i][j]=1;
            
            // if(i==0 || i==n-1) locked[i][j]=1;
            // else locked[i][j]=0;
            // u[i][j]=(i/(n-1.)-.5)*2*255;

            if(j==0 || j==m-1) locked[i][j]=1;
            else locked[i][j]=0;
            u[i][j]=(j/(m-1.)-.5)*2*255;
        }
    for(;tot>1e-2;)
    {
        for(i=0;i<n;i++)
            for(j=0;j<m;j++)
                if(locked[i][j]) t[i][j]=u[i][j];
                else
                    if(vis[i][j])
                    {
                        t[i][j]=0;
                        if(i==0 || !vis[i-1][j]) t[i][j]+=u[i+1][j]/2;
                        else if(i==n-1 || !vis[i+1][j]) t[i][j]+=u[i-1][j]/2;
                        else t[i][j]+=(u[i+1][j]+u[i-1][j])/4;
                        if(j==0 || !vis[i][j-1]) t[i][j]+=u[i][j+1]/2;
                        else if(j==m-1 || !vis[i][j+1]) t[i][j]+=u[i][j-1]/2;
                        else t[i][j]+=(u[i][j+1]+u[i][j-1])/4;
                    }
        tot=0;
        for(i=0;i<n;i++)
            for(j=0;j<m;j++)
                if(vis[i][j])
                {
                    tot+=fabs(t[i][j]-u[i][j]);
                    u[i][j]=t[i][j];
                }
    }
    tot=0;
    
    for(i=0;i<n;i++)
        if(vis[i][0])
            tot+=u[i][1]-u[i][0];

    for(i=0;i<m;i++)
        if(vis[0][i])
            tot+=u[1][i]-u[0][i];

    printf("%.6lf\n",tot);
}
inline unsigned char RD(int i,int j)
{
    return (!BL(i,j) && vis[i][j])?(u[i][j]>0)?(int)round(-u[i][j]+255):255:0;
    // return (!BL(i,j) && vis[i][j])?(u[i][j]>0)?(int)round(sqrt((-u[i][j]+255)*255)):255:0;
}
inline unsigned char GR(int i,int j)
{
    return (!BL(i,j) && vis[i][j])?(u[i][j]<0)?(int)round(u[i][j]+255):255:0;
    // return (!BL(i,j) && vis[i][j])?(u[i][j]<0)?(int)round(sqrt((u[i][j]+255)*255)):255:0;
}
inline unsigned char BL(int i,int j)
{
    // return (vis[i][j] && fabs(round(u[i][j]/25)-u[i][j]/25)<0.05)?255:0;
    const int dx[]={0,0,1,-1},dy[]={1,-1,0,0};
    int k,t=round(u[i][j]/25)*25;
    if(!vis[i][j]) return 0;
    for(k=0;k<4;k++)
        if(i+dx[k]>=0 && i+dx[k]<n && j+dy[k]>=0 && j+dy[k]<n && vis[i+dx[k]][j+dy[k]] && (t-u[i][j])*(t-u[i+dx[k]][j+dy[k]])<=0)
            return 255;
    return 0;
}

FILE *fp;
void pixel_write(int i, int j)
{
    static unsigned char color[3];
    color[0] = RD(i,j)&255;
    color[1] = GR(i,j)&255;
    color[2] = BL(i,j)&255;
    fwrite(color, 1, 3, fp);
}
int main()
{
    int i,j;
    fp = fopen("files/out.ppm","wb");
    init();
    fprintf(fp, "P6\n%d %d\n255\n", m, n);
    for(i=0;i<n;i++)
        for(j=0;j<m;j++)
            pixel_write(i,j);
    fclose(fp);
    return 0;
}
