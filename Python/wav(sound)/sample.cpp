#include <bits/stdc++.h>
using namespace std;
typedef long double db;
const int maxlogn=18;
const int maxn=(1<<maxlogn)|1;
const int framerate=65536;
const int nchannels=1;
const int speed=16384;
// const int maxn=10000;
const db tau=2*acosl(-1);
const int feq[]={0,
    68,72,76,84,88,92,96,104,
    112,116,124,132,140,148,156,164,
    176,184,196,208,220,232,248,260,
    276,292,312,328,348,368,392,416,
    440,468,492,524,556,588,624,660,
    700,740,784,832,880,932,988,1048,
    1108,1176,1244,1320,1396,1480,1568,1660,
    1760,1864,1976,2092,2216,2348,2488,2636,
    2792,2960,3136,3324,3520,3728,3952,4188,
    4436,4700,4980,5276,5588,5920,6272,6644,
    7040,7460,7904,8372,8868,9396,9956,10548,
    11176,11840,12544,13288,14080,14916,15804,16744,
};
const char s[]=" \n;()[]{}<=>0123456789abcdefghijklmnopqrstuvwxyz|&!^\"'+-*/%\\.,#$?:@_`~ABCDEFGHIJKLMNOPQRSTUVWXYZ";
//                                            F G A BC D EF G A BC D EF G
//                                                   ^
int data[maxn],n,pos[30000];
int rev[maxn],logn;
struct comp
{
    db a,b;
    comp(db a=0,db b=0):a(a),b(b) {}
    inline comp operator +(const comp &x) const { return comp(a+x.a,b+x.b); }
    inline comp operator -(const comp &x) const { return comp(a-x.a,b-x.b); }
    inline comp operator *(const comp &x) const { return comp(a*x.a-b*x.b,a*x.b+b*x.a); }
}w[maxn],a[maxn];
inline db abs(const comp &x) { return sqrtl(x.a*x.a+x.b*x.b); }
inline void calcrev(int logn)
{
    int i;
    for(rev[0]=0,i=1;i<(1<<logn);i++)
        rev[i]=(rev[i>>1]>>1)|((i&1)<<(logn-1));
}
inline void init()
{
    int i;
    for(i=0;i<=(1<<maxlogn);i++)
        w[i]=comp(cosl(i*tau/(1<<maxlogn)),sinl(i*tau/(1<<maxlogn)));
}
inline void DFT(comp *a,int logn,bool flag)
{
    int i,j,k,mid;
    comp x,y;
    for(i=0;i<(1<<logn);i++)
        if(rev[i]<i)
            swap(a[rev[i]],a[i]);
    for(i=1;i<=logn;i++)
        for(mid=1<<(i-1),j=0;j<(1<<logn);j+=1<<i)
            for(k=0;k<mid;k++)
            {
                x=a[j|k],y=a[j|k|mid]*w[flag?(1<<maxlogn)-(k<<(maxlogn-i)):(k<<(maxlogn-i))];
                a[j|k]=x+y,a[j|k|mid]=x-y;
            }
}
FILE *f,*out;
inline void make()
{
    srand(time(NULL));
    int i,j;
    for(i=0;s[i];i++) pos[s[i]]=i;
    db x;
    f=fopen("test.ans","r");
    for(n=0;(data[n]=fgetc(f))!=EOF;n++) if(data[n]=='\r') n--; data[n]=0;
    fclose(f);
    for(i=0;i<n;i++) data[i]=feq[pos[data[i]]];
    f=fopen("test.out","w");
    fprintf(f,"%d %d\n",nchannels,framerate);
    for(i=0;i<nchannels;fprintf(f,"\n"),i++)
        for(j=0,x=0;j<speed*n;j++)
        {
            fprintf(f,"%.0Lf ",(data[j/speed]?sinl(x):0)*5000+rand()%51-25);
            x+=tau/framerate*data[j/speed];
        }
    fclose(f);
    system("python3 ./files/save_wav.py ./files/out.wav < test.out"); system("echo > test.out");
}
inline void solve()
{
    int i,j,maxx;
    db maxy;
    init();
    system("python3 ./files/load_wav.py ./files/out.wav > test.in");
    f=fopen("test.in","r"),out=fopen("std.out","w");
    fscanf(f,"%d%d%d",&i,&j,&n); assert(i==nchannels && j==framerate);
    logn=14;
    calcrev(logn);
    for(i=0;s[i];i++) pos[feq[i]/4]=i;
    do 
    {
        for(i=0;i<(1<<logn);i++) fscanf(f,"%Lf",&a[i].a),a[i].b=0;
        DFT(a,logn,0);
        for(i=0,maxy=10000;i<5000;i++)
            if(abs(a[i])>maxy)
                maxx=i,maxy=abs(a[i]);
        if(maxy==10000) maxx=0;
        fputc(s[pos[maxx]],out),fflush(out);
    }while(n-=(1<<logn));
    fclose(f),fclose(out); system("echo > test.in");
}
int main()
{
    make(),solve();
    return 0;
}
