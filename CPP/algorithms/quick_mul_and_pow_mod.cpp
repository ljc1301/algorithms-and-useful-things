long long mod=100000003L;
long long mult_mod(long long a,long long b)
{
    long long res=0;  
    while(b)  
    {  
        if(b&1) res=(res+a)%mod;
        b>>=1;  
        a=(a<<1)%mod;  
    }  
    return res%mod;
}
long long quick_pow_mod(long long a,long long b)
{
    long long res=1,i;
    if(!b) return res;
    i=1; i<<=62;
    for(;i>b;i>>=1);
    for(;i>0;i>>=1)
    {
        res=mult_mod(res,res);
        if(b&i) res=mult_mod(res,a);
    }
    return res;
}
