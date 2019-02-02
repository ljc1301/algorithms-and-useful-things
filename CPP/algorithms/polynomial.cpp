const int maxlogn=20;
const int maxn=(1<<maxlogn)|1;
const int kcz=998244353;
typedef long long ll;
namespace polynomial
{
    ll G[2][24];
    int rev[maxn];
    inline ll qpow(ll a,int k)
    {
        ll res=1;
        while(k)
        {
            if(k&1) res=res*a%kcz;
            k>>=1;
            if(k) a=a*a%kcz;
        }
        return res;
    }
    #define inv(x) qpow(x,kcz-2)
    void init()
    {
        int i;
        G[1][23]=inv(G[0][23]=qpow(3,(kcz-1)>>23));
        for(i=22;i>=0;i--)
        {
            G[0][i]=G[0][i+1]*G[0][i+1]%kcz;
            G[1][i]=G[1][i+1]*G[1][i+1]%kcz;
        }
    }
    inline void calcrev(int logn)
    {
        register int i;
        rev[0]=0;
        for(i=1;i<(1<<logn);i++)
            rev[i]=(rev[i>>1]>>1)|((i&1)<<(logn-1));
    }
    inline void DFT(ll *a,int logn,int flag) // O(nlogn)
    {
        register int i,j,k,mid;
        register ll t1,t2,t;
        for(i=0;i<(1<<logn);i++)
            if(rev[i]<i)
                swap(a[rev[i]],a[i]);
        for(i=1;i<=logn;i++)
            for(mid=1<<(i-1),j=0;j<(1<<logn);j+=1<<i)
                for(k=0,t=1;k<mid;k++,t=t*G[flag][i]%kcz)
                {
                    t1=a[j|k],t2=t*a[j|k|mid];
                    a[j|k]=(t1+t2)%kcz,a[j|k|mid]=(t1-t2)%kcz;
                }
    }
    ll temp[maxn];
    inline void inverse(const ll *f,ll *g,int n) // mod x^n, O(nlogn)
    {
        int logn,i;
        long long t,t1=inv(2);
        for(logn=0,g[0]=inv(f[0]),t=t1;(1<<logn)<n;logn++)
        {
            memset(g+(1<<logn),0,sizeof(ll)*(3<<logn));
            calcrev(logn+2);
            DFT(g,logn+2,0);
            memcpy(temp,f,sizeof(ll)<<(logn+1));
            memset(temp+(1<<(logn+1)),0,sizeof(ll)<<(logn+1));
            DFT(temp,logn+2,0);
            for(i=0;i<(4<<logn);i++) temp[i]=(2*g[i]-g[i]*g[i]%kcz*temp[i])%kcz;
            DFT(temp,logn+2,1);
            for(i=0,(t*=t1)%=kcz;i<(2<<logn);i++)
                g[i]=t*temp[i]%kcz;
        }
        for(i=n;i<(1<<logn);i++)
            g[i]=0;
    }
    ll ig[maxn];
    inline void divide(ll *f,ll *g,ll *res,int n,int m) //n、m次, O(nlogn)
    {
        int logn,i;
        ll t;
        for(logn=0;(1<<logn)<2*n-m+1;logn++);
        reverse(f,f+n+1),reverse(g,g+m+1);
        for(i=m+1;i<(1<<logn);i++) g[i]=0;
        inverse(g,ig,n-m+1);
        for(i=n-m+1;i<(1<<logn);i++)  ig[i]=0;    
        memset(res,0,sizeof(ll)<<logn);
        calcrev(logn),memcpy(res,f,sizeof(ll)*(n+1)),DFT(res,logn,0),DFT(ig,logn,0);
        for(i=0;i<(1<<logn);i++) res[i]=res[i]*ig[i]%kcz;
        DFT(res,logn,1);
        for(i=0,t=inv(1<<logn);i<(1<<logn);i++)
            res[i]=t*res[i]%kcz;
        for(i=n-m+1;i<(1<<logn);i++)
            res[i]=0;
        reverse(res,res+n-m+1),reverse(f,f+n+1),reverse(g,g+m+1);
    }
    inline void modulo(ll *f,ll *g,ll *res,int n,int m) //n、m次，g发生改变, O(nlogn)
    {
        int i;
        for(i=n;i>=0 && !f[i];i--);
        if(m>i) { memcpy(res,f,sizeof(ll)*(n+1)); return; }
        int logn;
        ll t;
        divide(f,g,res,n,m);
        for(logn=0;(1<<logn)<=n;logn++);
        calcrev(logn),DFT(g,logn,0),DFT(res,logn,0);
        for(i=0;i<(1<<logn);i++) res[i]=res[i]*g[i]%kcz;
        DFT(res,logn,1);
        for(i=0,t=inv(1<<logn);i<m;i++)
            res[i]=(f[i]-res[i]*t)%kcz;
        for(;i<(1<<logn);i++) res[i]=0;
    }
    ll temp1[maxlogn][maxn],h[maxn];
    inline void query(ll *f,ll *a,ll *res,int n,int m) // n次, O(nlog^2n)
    {
        int i,logm,j,k,mid;
        ll t;
        while(n>=0 && !f[n]) n--;
        if(n<0) { for(i=0;i<m;i++) res[i]=0; return; }
        for(logm=0;(1<<logm)<m;logm++);
        for(i=0;i<(1<<logm);i++)
            temp1[0][i]=-a[i];
        for(i=1;i<=logm;i++)
            for(j=0,mid=1<<(i-1),calcrev(i),t=inv(1<<i);j<(1<<logm);j+=1<<i)
            {
                memcpy(h,temp1[i-1]+j,sizeof(ll)<<(i-1));
                memset(h+mid,0,sizeof(ll)<<(i-1));
                memcpy(temp,temp1[i-1]+(j|mid),sizeof(ll)<<(i-1));
                memset(temp+mid,0,sizeof(ll)<<(i-1));
                h[mid]=temp[mid]=1;
                DFT(h,i,0),DFT(temp,i,0);
                for(k=0;k<(1<<i);k++) temp[k]=h[k]*temp[k]%kcz;
                DFT(temp,i,1);
                for(k=0;k<(1<<i);k++) temp1[i][j|k]=temp[k]*t%kcz;
                temp1[i][j]=(temp1[i][j]-1)%kcz;
            }
        temp1[logm][1<<logm]=1;
        memset(h,0,sizeof(ll)<<logm);
        modulo(f,temp1[logm],h,n,1<<logm);
        memcpy(temp1[logm],h,sizeof(ll)<<logm);
        for(i=logm;i>=1;i--,memcpy(temp1[i],res,sizeof(ll)<<logm))
            for(j=(1<<logm)-(1<<i),mid=1<<(i-1);j>=0;j-=(1<<i))
            {
                temp1[i-1][j+(1<<i)]=1;
                modulo(temp1[i]+j,temp1[i-1]+(j|mid),h,min((1<<i)-1,n),mid);
                memcpy(res+(j|mid),h,sizeof(ll)<<(i-1));
                temp1[i-1][j|mid]=1;
                modulo(temp1[i]+j,temp1[i-1]+j,h,min((1<<i)-1,n),mid);
                memcpy(res+j,h,sizeof(ll)<<(i-1));
            }
    }
    ll temp2[maxlogn][maxn];
    inline void interpolate(ll *x,ll *y,ll *f,int n) // O(nlogn^2n)
    {
        int i,logn,j,k,mid;
        ll t;
        for(logn=0;(1<<logn)<n;logn++);
        for(i=0;i<n;i++)
            temp1[0][i]=-x[i];
        for(;i<(1<<logn);i++) temp1[0][i]=0;
        for(i=1;i<=logn;i++)
            for(j=0,mid=1<<(i-1),calcrev(i),t=inv(1<<i);j<(1<<logn);j+=1<<i)
                if((j|mid)<n)
                {
                    memcpy(h,temp1[i-1]+j,sizeof(ll)<<(i-1));
                    memset(h+mid,0,sizeof(ll)<<(i-1));
                    memcpy(temp,temp1[i-1]+(j|mid),sizeof(ll)<<(i-1));
                    memset(temp+mid,0,sizeof(ll)<<(i-1));
                    h[mid]=temp[min(j+(1<<i),n)-(j|mid)]=1;
                    DFT(h,i,0),DFT(temp,i,0);
                    for(k=0;k<(1<<i);k++) temp[k]=h[k]*temp[k]%kcz;
                    DFT(temp,i,1);
                    for(k=0;k<(1<<i);k++) temp1[i][j|k]=temp[k]*t%kcz;
                    if(j+(1<<i)<=n) temp1[i][j]=(temp1[i][j]-1)%kcz;
                    else temp1[i][n]=(temp1[i][n]-1)%kcz;
                }
                else
                    memcpy(temp1[i]+j,temp1[i-1]+j,sizeof(ll)<<i);
        for(i=0;i<=logn;i++)
            for(j=0;j<(1<<logn);j++)
                temp2[i][j]=temp1[i][j];
        temp1[logn][n]=1;
        for(i=0;i<n;i++)
            temp1[logn][i]=temp1[logn][i+1]*(i+1)%kcz;
        temp1[logn][n]=0;
        for(i=logn;i>=1;i--,memcpy(temp1[i],f,sizeof(ll)<<logn))
            for(j=(1<<logn)-(1<<i),mid=1<<(i-1);j>=0;j-=(1<<i))
                if((j|mid)<n)
                {
                    temp1[i-1][min(j+(1<<i),n)]=1;
                    modulo(temp1[i]+j,temp1[i-1]+(j|mid),h,min(1<<i,n-j)-1,min(mid,n-(j|mid)));
                    memcpy(f+(j|mid),h,sizeof(ll)<<(i-1));
                    temp1[i-1][j|mid]=1;
                    modulo(temp1[i]+j,temp1[i-1]+j,h,min(1<<i,n-j)-1,mid);
                    memcpy(f+j,h,sizeof(ll)<<(i-1));
                }
                else
                    memcpy(f+j,temp1[i]+j,sizeof(ll)<<i);
        for(i=0;i<n;i++) temp1[0][i]=y[i]*inv(temp1[0][i])%kcz;
        for(i=1;i<=logn;i++)
            for(j=0,mid=1<<(i-1),calcrev(i),t=inv(1<<i);j<(1<<logn);j+=1<<i)
                if((j|mid)<n)
                {
                    memcpy(h,temp1[i-1]+j,sizeof(ll)<<(i-1));
                    memset(h+mid,0,sizeof(ll)<<(i-1));
                    memcpy(temp,temp2[i-1]+(j|mid),sizeof(ll)<<(i-1));
                    memset(temp+mid,0,sizeof(ll)<<(i-1));
                    temp[min(mid,n-mid-j)]=1;
                    DFT(h,i,0),DFT(temp,i,0);
                    for(k=0;k<(1<<i);k++) temp[k]=h[k]*temp[k]%kcz;
                    DFT(temp,i,1);
                    for(k=0;k<(1<<i);k++) temp1[i][j|k]=temp[k]*t%kcz;
                    memcpy(h,temp2[i-1]+j,sizeof(ll)<<(i-1));
                    memset(h+mid,0,sizeof(ll)<<(i-1));
                    memcpy(temp,temp1[i-1]+(j|mid),sizeof(ll)<<(i-1));
                    memset(temp+mid,0,sizeof(ll)<<(i-1));
                    h[mid]=1;
                    DFT(h,i,0),DFT(temp,i,0);
                    for(k=0;k<(1<<i);k++) temp[k]=h[k]*temp[k]%kcz;
                    DFT(temp,i,1);
                    for(k=0;k<(1<<i);k++) (temp1[i][j|k]+=temp[k]*t)%=kcz;
                }
            else memcpy(temp1[i]+j,temp1[i-1]+j,sizeof(ll)<<i);
        for(i=0;i<n;i++) f[i]=temp1[logn][i];
        for(;i<(1<<logn);i++) f[i]=0;
    }
    inline void polyln(const ll *f,ll *g,int n) // mod x^n, O(nlogn), assert(f[0]==1)
    {
        int i,logn;
        ll t;
        for(logn=0;(1<<logn)<(n<<1);logn++);
        inverse(f,g,n);
        for(i=0;i<n-1;i++)
            temp[i]=f[i+1]*(i+1)%kcz;
        temp[n-1]=0;
        for(i=n;i<(1<<logn);i++)
            temp[i]=g[i]=0;
        calcrev(logn),DFT(g,logn,0),DFT(temp,logn,0);
        for(i=0;i<(1<<logn);i++) temp[i]=g[i]*temp[i]%kcz;
        DFT(temp,logn,1);
        for(t=inv(1<<logn),i=1,g[0]=0;i<n;i++)
            g[i]=temp[i-1]*inv(i)%kcz*t%kcz;
    }
    inline void polyexp(const ll *f,ll *g,int n) // mod x^n, O(nlogn), assert(f[0]==0)
    {
        int logn,i;
        long long t,t1=inv(2);
        for(logn=0,g[0]=1,t=t1;(1<<logn)<n;logn++)
        {
            memset(g+(1<<logn),0,sizeof(ll)*(3<<logn));
            polyln(g,h,2<<logn);
            memset(h+(2<<logn),0,sizeof(ll)*(2<<logn));
            for(i=0;i<(2<<logn);i++) h[i]=(f[i]+(!i)-h[i])%kcz;
            calcrev(logn+2);
            DFT(h,logn+2,0),DFT(g,logn+2,0);
            for(i=0;i<(4<<logn);i++) h[i]=h[i]*g[i]%kcz;
            DFT(h,logn+2,1);
            for(i=0,(t*=t1)%=kcz;i<(2<<logn);i++)
                g[i]=t*h[i]%kcz;
        }
        for(i=n;i<(1<<logn);i++)
            g[i]=0;
    }
    inline void polysqrt(const ll *f,ll *g,int n) // mod x^n, O(nlogn), assert(f[0]==1)
    {
        int logn,i;
        long long t,t1=inv(2);
        for(logn=0,g[0]=1,t=t1;(1<<logn)<n;logn++)
        {
            memset(g+(1<<logn),0,sizeof(ll)*(3<<logn));
            inverse(g,ig,2<<logn);
            memset(ig+(2<<logn),0,sizeof(ll)*(2<<logn));
            memcpy(h,f,sizeof(ll)*(2<<logn));
            memset(h+(2<<logn),0,sizeof(ll)*(2<<logn));
            calcrev(logn+2);
            DFT(ig,logn+2,0),DFT(h,logn+2,0);
            for(i=0;i<(4<<logn);i++) h[i]=h[i]*ig[i]%kcz;
            DFT(h,logn+2,1);
            for(i=0,(t*=t1)%=kcz;i<(2<<logn);i++)
                g[i]=(t*h[i]+g[i])%kcz*t1%kcz;
        }
        for(i=n;i<(1<<logn);i++)
            g[i]=0;
    }
    #undef inv
}
