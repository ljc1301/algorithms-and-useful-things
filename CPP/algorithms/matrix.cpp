#include <cstring>
#include <algorithm>
using namespace std;
const int maxn=100;
struct matrix
{
    int row,col;
    double a[maxn][maxn];
    matrix() { memset(a,0,sizeof(a)); }
    bool getI()
    {
        int i;
        if(row!=col) return false;
        memset(a,0,sizeof(a));
        for(i=0;i<row;i++)
            a[i][i]=1;
        return true;
    }
    void getI(int sz)
    {
        int i;
        row=col=sz;
        memset(a,0,sizeof(a));
        for(i=0;i<sz;i++)
            a[i][i]=1;
    }
    matrix operator *(const matrix& ma)
    {
        matrix ans;
        int i,j,l;
        if(col!=ma.row) return ans;
        ans.row=row;
        ans.col=ma.col;
        for(i=0;i<ans.row;i++)
            for(j=0;j<ans.col;j++)
                for(l=0;l<col;l++)
                    ans.a[i][j]+=a[i][l]*ma.a[l][j];
        return ans;
    }
    matrix operator *=(const matrix& ma)
    {
        return *this=*this*ma;
    }
    bool GetMatrixInverse(matrix& ans)
    {
        double flag=getA();
        matrix t;
        int i,j;
        if(col!=row) return false;
        if(flag==0)
        return false;
        getAStart(t);
        ans.col=ans.row=col;
        for(i=0;i<col;i++)
            for(j=0;j<col;j++)
                ans.a[i][j]=t.a[i][j]/flag;
        return true;
    }
    double getA()
    {
        if(col!=row) return 0;
        if(col==1) return a[0][0];
        double ans=0;
        matrix temp; temp.col=temp.row=col-1;
        int i,j,k;
        for(i=0;i<col;i++)
        {
            for(j=0;j<col-1;j++)
                for(k=0;k<col-1;k++)
                    temp.a[j][k]=a[j+1][(k>=i)?k+1:k];
            double t=temp.getA();
            if(i%2==0)
                ans+=a[0][i]*t;
            else
                ans-=a[0][i]*t;
        }
        return ans;
    }
    bool getAStart(matrix& ans)
    {
        if(col!=row) return false;
        ans.col=ans.row=col; 
        if(col==1)
        {
            ans.a[0][0]=1;
            return true;
        }
        int i,j,k,t;
        matrix temp;
        temp.col=temp.row=col-1;
        for(i=0;i<col;i++)
            for(j=0;j<col;j++)
            {
                for(k=0;k<col-1;k++)
                    for(t=0;t<col-1;t++)
                        temp.a[k][t]=a[k>=i?k+1:k][t>=j?t+1:t];
                ans.a[j][i]=temp.getA();
                if((i+j)%2==1)
                    ans.a[j][i]=-ans.a[j][i];
            }
        return true;
    }
    matrix quickPow(long long b)
    {
        matrix ans;
        long long i;
        ans.row=ans.col=row;
        if(!ans.getI() || row!=col) return ans;
        if(b==0) return ans;
        i=1; i<<=62;
        for(;i>b;i>>=1);
        for(;i>0;i>>=1)
        {
            ans*=ans;
            if(b&i) ans*=*this;
        }
        return ans;
    }
    matrix gauss_jordan(const matrix& A)
    {
        int n=row,i,j,k;
        matrix res,B;
        if(row!=col || col!=A.row || A.col!=1) return res;
        memcpy(&B,this,sizeof(*this));
        res.row=row; res.col=1; B.col++;
        for(i=0;i<n;i++) B.a[i][n]=A.a[i][0];
        for(i=0;i<n;i++)
        {
            int pmaxi=i;
            for(j=i+1;j<n;j++)
                if(abs(B.a[j][i])>abs(B.a[pmaxi][i]))
                    pmaxi=j;
            for(j=0;j<=n;j++)
                swap(B.a[pmaxi][j],B.a[i][j]);
            if(abs(B.a[i][i])<eps) return res;
            for(j=0;j<=n;j++) if(i!=j) B.a[i][j]/=B.a[i][i];
            B.a[i][i]=1;
            for(j=0;j<n;j++)
                if(i!=j)
                {
                    for(k=0;k<=n;k++)
                        if(k!=i)
                            B.a[j][k]-=B.a[j][i]*B.a[i][k];
                    B.a[j][i]=0;
                }
        }
        for(i=0;i<n;i++)
            res.a[i][0]=B.a[i][n];
        return res;
    }
};
