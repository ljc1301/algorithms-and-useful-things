#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
using namespace std;
const int maxlen=1000000;
const int maxstack=1000000;
const int maxn=1000;
const int maxtime=10000000;
short stack[maxstack],program[maxn][maxn];
//short input[maxlen],output[maxlen];
int len,top,pos;
//short inputlen;
short lines,prol[maxn];
void push(short val)
{
    if(len>=maxstack)
    {
        printf("Stack overflow!\n");
        exit(0);
    }
    stack[len++]=val;
}
short pop()
{
    if(!len) return 0;
    return stack[--len];
}
void writeint(short a)
{
    // short temp[30],top=0;
    // bool flag=a<0;
    // if(a<0) a=-a;
    // if(a==0)
    // {
    //     if(len+2>maxlen)
    //     {
    //         printf("Output limit exceeded!\n");
    //         exit(0);
    //     }
    //     output[len++]='0';
    //     output[len++]=' ';
    //     return;
    // }
    // while(a)
    // {
    //     temp[top++]=a%10;
    //     a/=10;
    // }
    // if(len+top+1+flag>maxlen)
    // {
    //     printf("Output limit exceeded!\n");
    //     exit(0);
    // }
    // if(flag) output[len++]='-';
    // while(top--)
    //     output[len++]=temp[top];
    // output[len++]=' ';
    printf("%hd ",a);
}
void writechar(short val)
{
    // if(len>=maxlen)
    // {
    //     printf("Output limit exceeded!\n");
    //     exit(0);
    // }
    // output[len++]=val;
    printf("%c",val);
}
short readchar()
{
    // if(pos>=inputlen) return 0;
    // return input[pos++];
    return getchar();
}
short readint()
{
    // short res;
    // while(pos<=inputlen && (input[pos]<'0' || input[pos]>'9'))
    //     pos++;
    // if(pos>inputlen) return 0;
    // res=input[pos++]-'0';
    // while(pos<=inputlen && input[pos]>='0' && input[pos]<='9')
    //     res=res*10+input[pos++]-'0';
    // return res;
    short res;
    scanf("%hd",&res);
    return res;
}
const int ne[4][2]={{-1,0},{1,0},{0,-1},{0,1}};
void work()
{
    int x=0,y=0,dir=3,cnt=0,time=0;
    bool stringmode=false,skip=false;
    short a,b,c;
    len=top=0;
    while(true)
    {
        if(!skip)
        {
            time++;
            if(time>=maxtime)
            {
                printf("Time limit exceeded!\n");
                exit(0);
            }
        }
        if(skip) skip=false;
        else if(stringmode)
            if(program[x][y]=='\"')
                stringmode=false;
            else push(program[x][y]);
        else
            switch(program[x][y])
            {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    push(program[x][y]-'0');
                    break;
                case '@': return;
                case '$': pop(); break;
                case ':': if(len==0) push(0); else push(stack[len-1]); break;
                case '\\':
                    a=pop(); b=pop();
                    push(a); push(b);
                    break;
                case '+': push(pop()+pop()); break;
                case '-':
                    a=pop(); b=pop();
                    push(b-a);
                    break;
                case '*': push(pop()*pop()); break;
                case '/':
                    a=pop(); b=pop();
                    push(b/a);
                    break;
                case '%':
                    a=pop(); b=pop();
                    push(b%a);
                    break;
                case '\"': stringmode=true; break;
                case '!': push(!pop()); break;
                case '`': push(pop()<pop()); break;
                case '.': writeint(pop()); break;
                case ',': writechar(pop()); break;
                case '~': push(readchar()); break;
                case '&': push(readint()); break;
                case '#': skip=true; break;
                case '^': dir=0; break;
                case 'v': dir=1; break;
                case '<': dir=2; break;
                case '>': dir=3; break;
                case '|': dir=pop()?0:1; break;
                case '_': dir=pop()?2:3; break;
                case '?': dir=rand()%4; break;
                case 'g':
                    a=pop(); b=pop();
                    if(a<0 || a>=lines) push(0);
                    else if(b<0 || b>=prol[a])
                        push(0);
                    else push(program[a][b]);
                    break;
                case 'p':
                    a=pop(); b=pop(); c=pop();
                    if(a<0) break;
                    if(a>=maxn) break;
                    if(b<0) break;
                    if(b>=maxn) break;
                    lines=max(lines,a);
                    prol[a]=max(prol[a],b);
                    program[a][b]=c;
                    break;
            }
        x+=ne[dir][0]; y+=ne[dir][1];
        if(x<0) x+=lines;
        if(x>=lines) x-=lines;
        if(y<0) y+=prol[x];
        if(y>=prol[x]) y-=prol[x];
    }
}
int main()
{
    int i,j;
    char temp[maxn];
    fgets(temp,maxn,stdin);
    sscanf(temp,"%hd",&lines);
    for(i=0;i<lines;i++)
    {
        fgets(temp,maxn,stdin);
        prol[i]=strlen(temp);
        for(j=0;j<prol[i];j++)
            program[i][j]=temp[j];
    }
    work();
    return 0;
}
