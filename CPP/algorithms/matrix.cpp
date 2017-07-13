#include <cstring>
const int maxn=100;
struct matrix
{
    int row,col;
    double a[maxn][maxn];
    matrix() { memset(a,0,sizeof(a)); }
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
};
