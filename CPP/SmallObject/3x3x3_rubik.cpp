#include <cstdio>
#include <cstring>
#include <set>
using namespace std;
const int sc[20][3]=
{
    {4*9+0,0*9+0,3*9+2},
    {4*9+2,3*9+0,2*9+2},
    {4*9+8,2*9+0,1*9+2},
    {4*9+6,1*9+0,0*9+2},
    {5*9+6,3*9+8,0*9+6},
    {5*9+8,2*9+8,3*9+6},
    {5*9+2,1*9+8,2*9+6},
    {5*9+0,0*9+8,1*9+6},
    {4*9+1,3*9+1,-1},
    {4*9+3,0*9+1,-1},
    {4*9+5,2*9+1,-1},
    {4*9+7,1*9+1,-1},
    {5*9+1,1*9+7,-1},
    {5*9+3,0*9+7,-1},
    {5*9+5,2*9+7,-1},
    {5*9+7,3*9+7,-1},
    {0*9+5,1*9+3,-1},
    {1*9+5,2*9+3,-1},
    {2*9+5,3*9+3,-1},
    {3*9+5,0*9+3,-1},
};
const int faces[6][8]={
    {0,9,3,16,7,13,4,19},
    {3,11,2,17,6,12,7,16},
    {2,10,1,18,5,14,6,17},
    {1,8,0,19,4,15,5,18},
    {0,8,1,10,2,11,3,9},
    {7,12,6,14,5,15,4,13},
};
struct cube
{
    char col[54]; // L F R B U D
    int h() const
    {
        int res=0,i,j,r1,r2;
        for(i=0;i<6;i++)
            for(j=0;j<8;j++)
            {
                for(r1=0;;r1++)
                    if(sc[faces[i][j]][r1]/9==i) break;
                for(r2=0;;r2++)
                    if(sc[faces[i][(j+1)%8]][r2]/9==i) break;
                if(col[sc[faces[i][j]][r1]]!=col[sc[faces[i][(j+1)%8]][r2]])
                {
                    res++;
                    continue;
                }
                if(j&1)
                {
                    r1^=1;
                    for(r2=0;;r2++)
                        if(sc[faces[i][(j+1)%8]][r2]/9==sc[faces[i][j]][r1]/9) break;
                }
                else
                {
                    r2^=1;
                    for(r1=0;;r1++)
                        if(sc[faces[i][(j+1)%8]][r2]/9==sc[faces[i][j]][r1]/9) break;
                }
                if(col[sc[faces[i][j]][r1]]!=col[sc[faces[i][(j+1)%8]][r2]])
                    res++;
            }
        res/=2;
        for(i=0;i<6;i++)
            for(j=1;j<8;j+=2)
                if(sc[faces[i][j]][0]/9==i)
                {
                    if(col[sc[faces[i][j]][0]]!=col[i*9+4])
                        res++;
                }
                else
                {
                    if(col[sc[faces[i][j]][1]]!=col[i*9+4])
                        res++;
                }
        return res;
    }
    bool operator <(const cube& n) const
    {
        return strcmp(col,n.col)<0;
    }
    int getface(char ch) const
    {
        switch(ch)
        {
            case 'L': return 0;
            case 'F': return 1;
            case 'R': return 2;
            case 'B': return 3;
            case 'U': return 4;
            case 'D': return 5;
        }
        return -1;
    }
    cube turn(const char *str) const
    {
        int f=getface(str[0]),i,j,c=0,r1,r2;
        if(f==-1) return *this;
        cube ret;
        memcpy(ret.col,col,sizeof(col));
        if(str[1]=='\0') c=2;
        else if(str[1]=='\'') c=6;
        else if(str[1]=='2') c=4;
        for(i=0;i<8;i++)
        {
            for(r1=0;;r1++)
                if(sc[faces[f][i]][r1]/9==f) break;
            for(r2=0;;r2++)
                if(sc[faces[f][(i+c)%8]][r2]/9==f) break;
            for(j=0;j<((i&1)?2:3);j++)
                ret.col[sc[faces[f][(i+c)%8]][(j+r2)%((i&1)?2:3)]]=
                    col[sc[faces[f][i]][(j+r1)%((i&1)?2:3)]];
        }
        return ret;
    }
    bool read()
    {
        char temp[15];
        int i,j;
        for(i=0;i<3;i++)
            if(scanf("%s",col+4*9+i*3)!=1)
                return false;
        for(i=0;i<3;i++)
        {
            if(scanf("%s",temp)!=1)
                return false;
            for(j=0;j<4;j++)
                memcpy(col+j*9+i*3,temp+j*3,3);
        }
        for(i=0;i<3;i++)
            if(scanf("%s",col+5*9+i*3)!=1)
                return false;
        return true;
    }
    void print() const
    {
        int i,j,k;
        for(i=0;i<3;i++)
        {
            printf("   ");
            for(j=0;j<3;j++)
                printf("%c",col[4*9+i*3+j]);
            printf("\n");
        }
        for(i=0;i<3;i++)
        {
            for(j=0;j<4;j++)
                for(k=0;k<3;k++)
                    printf("%c",col[j*9+i*3+k]);
            printf("\n");
        }
        for(i=0;i<3;i++)
        {
            printf("   ");
            for(j=0;j<3;j++)
                printf("%c",col[5*9+i*3+j]);
            printf("\n");
        }
    }
}c;
const int maxdep=10;
const char turns[6]={'L','F','R','B','U','D'};
const char times[3]={'\0','\'','2'};
set<cube> se;
int dep;
char ans[maxdep][3];
bool ida(int d,const cube& c)
{
    int i,j;
    cube ne;
    if(se.count(c)) return false;
    if((dep-d)*8<c.h()) return false;
    se.insert(c);
    if(d>=dep) return true;
    for(i=0;i<6;i++)
    {
        ans[d][0]=turns[i];
        if(d && ans[d-1][0]==ans[d][0]) continue;
        for(j=0;j<3;j++)
        {
            ans[d][1]=times[j];
            ne=c.turn(ans[d]);
            if(ida(d+1,ne)) return true;
        }
    }
    se.erase(c);
    return false;
}
int main()
{
#ifdef SHOW
    int i,j;
    for(i=0;i<6;i++)
        c.col[i*9+4]=' ';
    for(i=0;i<20;i++)
        for(j=0;j<3 && sc[i][j]!=-1;j++)
            c.col[sc[i][j]]=(i<10)?i+'0':i-10+'A';
    c.print();
#else
#ifdef TURN
    char str[3];
    c.read();
    while(scanf("%s",str)==1)
    {
        c=c.turn(str);
        c.print();
    }
#else
    while(c.read())
    {
#ifndef H
        int i;
        se.clear();
        for(dep=0;dep<=maxdep;dep++)
        {
            printf("searching %d\n",dep);
            if(ida(0,c))
                break;
        }
        if(dep>maxdep)
            printf("Cannot finish it in %d steps.\n",maxdep);
        else
        {
            int i;
            printf("%d step(s):",dep);
            for(i=0;i<dep;i++)
                printf(" %s",ans[i]);
            printf("\n");
        }
#else
        printf("%d\n",c.h());
#endif
    }
#endif
#endif
    return 0;
}
