#include <bits/stdc++.h>
using namespace std;
const int maxmemory=(1<<21)|1;
const int maxlen=10005;

int memory[maxmemory],last_p,tot_var;
stack<pair<int,vector<int> > > var[maxlen]; // pointer, size
map<string,int> var_id;
vector<pair<int,int> > using_;

enum exp_type
{
    not_,posi,nega,times,divide,modulo,plus_,minus_,le,ge,l_,g_,eq,neq,xor_,and_,or_,set_,
    number,varible,declare,invoke,block,return_,if_,while_,for_,cin_,cout_,
};
const int priority[]={8,8,8,7,7,7,6,6,5,5,5,5,4,4,3,2,1,0};
struct expression
{
    exp_type t;
    vector<int> param;
}exp_[maxlen];
int tot_exp;

struct function
{
    int body;
    vector<int> param;
}func_[maxlen];
int tot_func;
map<string,int> func_id;

int input[maxlen],n,pos;

pair<int,int> evaluate(const expression &e,int dep) // value, address(-2: return; -1: none; -100: endl)
{
    int i,j,k,l,t;
    pair<int,int> res;
    switch(e.t)
    {
        case number:
            return make_pair(e.param[0],-1);
        case varible:
            if(e.param[0]==-100) return make_pair(0,-100);
            t=e.param[0],k=var[t].top().first;
            for(i=1;i<e.param.size();i++)
                k+=evaluate(exp_[e.param[i]],dep+1).first*var[t].top().second[i-1];
            return make_pair(memory[k],k);
        case declare:
            {
                vector<int> temp;
                for(i=0;i<e.param.size();i++)
                {
                    temp.clear();
                    k=e.param[i];
                    for(i++,l=1;e.param[i]!=-1;i++)
                    {
                        if(l>1) temp.push_back(e.param[i]);
                        l*=e.param[i];
                    }
                    for(j=temp.size()-2;j>=0;j--)
                        temp[j]*=temp[j+1];
                    if(l>1) temp.push_back(1);
                    using_.push_back(make_pair(dep,k));
                    var[k].push(make_pair(last_p,temp));
                    memset(memory+last_p,0,sizeof(int)*l);
                    last_p+=l;
                }
                temp.clear();
                break;
            }
        case invoke:
            if(e.param[0]==-1) { putchar(evaluate(exp_[e.param[1]],dep+1).first); break; }
            {
                vector<int> temp;
                temp.clear();
                k=last_p;
                for(i=1;i<e.param.size();i++)
                    temp.push_back(evaluate(exp_[e.param[i]],dep+1).first);
                for(i=1;i<e.param.size();i++)
                {
                    using_.push_back(make_pair(dep+1,func_[e.param[0]].param[i-1]));
                    memory[last_p]=temp[i-1];
                    var[func_[e.param[0]].param[i-1]].push(make_pair(last_p++,vector<int>()));
                }
                temp.clear();
                res=evaluate(exp_[func_[e.param[0]].body],dep+1);
                for(;!using_.empty() && using_.back().first>dep;using_.pop_back())
                    var[using_.back().second].pop();
                last_p=k;
                return make_pair((res.second==-2)?res.first:0,-1);
            }
        case block:
            k=last_p;
            for(i=0;i<e.param.size();i++)
            {
                res=evaluate(exp_[e.param[i]],dep+1);
                if(res.second==-2)
                {
                    last_p=k;
                    for(;!using_.empty() && using_.back().first>=dep;using_.pop_back())
                        var[using_.back().second].pop();
                    return res;
                }
            }
            for(;!using_.empty() && using_.back().first>=dep;using_.pop_back())
                var[using_.back().second].pop();
            break;
        case return_: return make_pair(evaluate(exp_[e.param[0]],dep+1).first,-2);
        case if_:
            if(evaluate(exp_[e.param[0]],dep+1).first)
            {
                res=evaluate(exp_[e.param[1]],dep+1);
                if(res.second==-2)
                    return res;
            }
            else if(e.param.size()>=3)
            {
                res=evaluate(exp_[e.param[2]],dep+1);
                if(res.second==-2)
                    return res;
            }
            break;
        case while_:
            while(evaluate(exp_[e.param[0]],dep+1).first)
            {
                res=evaluate(exp_[e.param[1]],dep+1);
                if(res.second==-2)
                    return res;
            }
            break;
        case for_:
            for(evaluate(exp_[e.param[0]],dep+1);evaluate(exp_[e.param[1]],dep+1).first;evaluate(exp_[e.param[2]],dep+1))
            {
                res=evaluate(exp_[e.param[3]],dep+1);
                if(res.second==2)
                {
                    for(;!using_.empty() && using_.back().first>dep;using_.pop_back())
                        var[using_.back().second].pop();
                    return res;
                }
            }
            for(;!using_.empty() && using_.back().first>dep;using_.pop_back())
                var[using_.back().second].pop();
            break;
        case cin_:
            for(i=0;i<e.param.size();i++)
                memory[evaluate(exp_[e.param[i]],dep+1).second]=input[pos++];
            break;
        case cout_:
            for(i=0;i<e.param.size();i++)
            {
                res=evaluate(exp_[e.param[i]],dep+1);
                if(res.second==-100) printf("\n");
                else printf("%d",res.first);
            }
            break;
#define param0 evaluate(exp_[e.param[0]],dep+1).first
#define param1 evaluate(exp_[e.param[1]],dep+1).first
        case not_: return make_pair(!param0,-1);
        case posi: return make_pair(param0,-1);
        case nega: return make_pair(-param0,-1);
        case times: return make_pair(param0*param1,-1);
        case divide: return make_pair(param0/param1,-1);
        case modulo: return make_pair(param0%param1,-1);
        case plus_: return make_pair(param0+param1,-1);
        case minus_: return make_pair(param0-param1,-1);
        case le: return make_pair(param0<=param1,-1);
        case ge: return make_pair(param0>=param1,-1);
        case l_: return make_pair(param0<param1,-1);
        case g_: return make_pair(param0>param1,-1);
        case eq: return make_pair(param0==param1,-1);
        case neq: return make_pair(param0!=param1,-1);
        case xor_: return make_pair((!!param0)^(!!param1),-1);
        case and_: return make_pair(param0 && param1,-1);
        case or_: return make_pair(param0 || param1,-1);
        case set_:
            res=evaluate(exp_[e.param[0]],dep+1);
            return make_pair(memory[res.second]=param1,res.second);
    }
    return make_pair(0,-1);
}
char program[maxlen];
int len,sta[maxlen],top,l[maxlen],r[maxlen],ne[maxlen];
inline void read()
{
    int i,ch;
    scanf("%d",&n);
    for(i=0;i<n;i++) scanf("%d",&input[i]);
    pos=0;
    for(i=0;i<52;i++)
        for(;!isgraph(ch=getchar()););
    for(len=0,program[len++]=' ';(ch=getchar())!=EOF;)
        program[len++]=ch;
    program[len]='\0';
}
#define str(s) string(s)
inline string slice_(int l,int r)
{ char t=program[r+1]; program[r+1]='\0'; string res(program+l); program[r+1]=t; return res; }
inline int to_int(string s) { int x; sscanf(s.c_str(),"%d",&x); return x; }
inline void prework()
{
    int i,t=0,j,pre,k=0; // 0: symbol; 1: number; 2: name
    string s;
    var_id.clear(),func_id.clear(),using_.clear(),last_p=tot_var=tot_func=0,tot_exp=1;
    exp_[0].t=number,exp_[0].param.clear(),exp_[0].param.push_back(1);
    var_id[str("endl")]=-100,func_id[str("putchar")]=-1;
    var_id[str("int")]=var_id[str("else")]=var_id[str("return")]=var_id[str("cin")]=var_id[str("cout")]=-233,var_id[str("endl")]=-100;
    func_id[str("if")]=func_id[str("for")]=func_id[str("while")]=func_id[str("return")]=-233;
    for(i=0;i<=len;i++) l[i]=r[i]=i;
    for(i=0,top=-1;i<=len;i++)
    {
        if(isgraph(program[i])) ne[k]=i,k=i;
        if(!t)
        {
            if(isdigit(program[i])) pre=i,t=1;
            else if(program[i]=='_' || isalpha(program[i])) pre=i,t=2;
        }
        else if(t==1)
        {
            if(!isdigit(program[i]))
                t=0,l[i-1]=pre,r[pre]=i-1;
        }
        else if(t==2)
            if(!(program[i]=='_' || isalnum(program[i])))
            {
                t=0,l[i-1]=pre,r[pre]=i-1;
                s=slice_(pre,i-1);
                for(j=i;j<=len;j++)
                    if(program[j]=='(')
                        break;
                    else if(isgraph(program[j]))
                        break;
                if(program[j]=='(' && !func_id.count(s)) func_id[s]=tot_func++;
                else if(program[j]!='(' && !var_id.count(s)) var_id[s]=tot_var++;
            }
        if(program[i]=='(' || program[i]=='[' || program[i]=='{') sta[++top]=i;
        if(program[i]==')' || program[i]==']' || program[i]=='}') r[sta[top]]=i,l[i]=sta[top],top--;
    }
    ne[k]=len+1;
}
int solve_expression(int,int);
int solve_string(int &L)
{
    int e=tot_exp++;
    exp_[e].param.clear();
    if(isdigit(program[L]))
    {
        exp_[e].t=number;
        exp_[e].param.push_back(to_int(slice_(L,r[L])));
        L=ne[r[L]];
    }
    else
    {
        string s=slice_(L,r[L]);
        L=ne[r[L]];
        if(program[L]=='(')
        {
            exp_[e].t=invoke;
            exp_[e].param.push_back(func_id[s]);
            for(;program[L]!=')';)
            {
                int pre=L=ne[L];
                if(program[L]==')') break;
                for(;program[L]!=')' && program[L]!=',';L=ne[(program[L]=='(')?r[L]:L]);
                exp_[e].param.push_back(solve_expression(pre,L-1));
            }
            L=ne[L];
        }
        else
        {
            exp_[e].t=varible;
            exp_[e].param.push_back(var_id[s]);
            for(;program[L]=='[';L=ne[r[L]])
                exp_[e].param.push_back(solve_expression(L+1,r[L]-1));
        }
    }
    return e;
}
inline int new_expression(exp_type t,int a,int b=0)
{
    int cur=tot_exp++;
    exp_[cur].t=t;
    exp_[cur].param.clear(),exp_[cur].param.push_back(a);
    if(priority[t]<8) exp_[cur].param.push_back(b);
    return cur;
}
int solve_expression(int L,int R)
{
    if(L>R) return 0;
    stack<int> e;
    stack<exp_type> op;
    int a,b;
    exp_type cur;
    while(!e.empty()) e.pop();
    while(!op.empty()) op.pop();
    for(;L<=R;)
        if(program[L]=='(')
        {
            e.push(solve_expression(ne[L],r[L]-1));
            L=ne[r[L]];
            while(!op.empty() && priority[op.top()]==8)
                a=e.top(),e.pop(),e.push(new_expression(op.top(),a)),op.pop();
        }
        else if(program[L]=='_' || isdigit(program[L]) || isalnum(program[L]))
        {
            e.push(solve_string(L));
            while(!op.empty() && priority[op.top()]==8)
                a=e.top(),e.pop(),e.push(new_expression(op.top(),a)),op.pop();
        }
        else
        {
            switch(program[L])
            {
                case '!': cur=(program[L+1]=='=')?L++,neq:not_; break;
                case '+': cur=(e.size()>op.size())?plus_:posi; break;
                case '-': cur=(e.size()>op.size())?minus_:nega; break;
                case '*': cur=times; break;
                case '/': cur=divide; break;
                case '%': cur=modulo; break;
                case '<': cur=(program[L+1]=='=')?L++,le:l_; break;
                case '>': cur=(program[L+1]=='=')?L++,ge:g_; break;
                case '=': cur=(program[L+1]=='=')?L++,eq:set_; break;
                case '^': cur=xor_; break;
                case '&': L++,cur=and_; break;
                case '|': L++,cur=or_; break;
            }
            while(!op.empty() && priority[cur]<=priority[op.top()] && priority[cur]<8 && cur!=set_)
                a=e.top(),e.pop(),b=e.top(),e.pop(),e.push(new_expression(op.top(),b,a)),op.pop();
            op.push(cur);
            L=ne[L];
        }
    while(!op.empty())
        a=e.top(),e.pop(),b=e.top(),e.pop(),e.push(new_expression(op.top(),b,a)),op.pop();
    a=e.top(),e.pop();
    assert(e.empty()),assert(op.empty());
    return a;
}
inline int solve_statement(int L,int R)
{
    if(L>R) return 0;
    if(program[L]=='(') return solve_expression(L,R);
    string s=slice_(L,r[L]);
    int cur,t;
    if(s=="int")
    {
        vector<int> temp;
        cur=tot_exp++;
        exp_[cur].t=declare;
        exp_[cur].param.clear();
        L=ne[r[L]];
        for(;program[L]!=';';)
        {
            temp.clear();
            if(program[L]==',') L=ne[L];
            s=slice_(L,r[L]);
            exp_[cur].param.push_back(var_id[s]);
            for(L=ne[r[L]];program[L]=='[';L=ne[r[L]])
                exp_[cur].param.push_back(to_int(slice_(L+1,r[L]-1)));
            exp_[cur].param.push_back(-1);
        }
        temp.clear();
        return cur;
    }
    else if(s=="return")
    {
        cur=tot_exp++;
        exp_[cur].t=return_;
        exp_[cur].param.clear();
        exp_[cur].param.push_back(solve_expression(ne[r[L]],R-1));
        return cur;
    }
    else if(s=="cin")
    {
        cur=tot_exp++;
        exp_[cur].t=cin_;
        exp_[cur].param.clear();
        for(L=ne[r[L]];L<=R && program[L]!=';';)
        {
            for(t=L=ne[ne[L]];L<=R && program[L]!=';' && (program[L]!='>' || program[L+1]!='>');L=ne[L]);
            exp_[cur].param.push_back(solve_expression(t,L-1));
        }
        return cur;
    }
    else if(s=="cout")
    {
        cur=tot_exp++;
        exp_[cur].t=cout_;
        exp_[cur].param.clear();
        for(L=ne[r[L]];L<=R && program[L]!=';';)
        {
            for(t=L=ne[ne[L]];L<=R && program[L]!=';' && (program[L]!='<' || program[L+1]!='<');L=ne[L]);
            exp_[cur].param.push_back(solve_expression(t,L-1));
        }
        return cur;
    }
    else return solve_expression(L,R-1);
}
inline int find_semicolon(int i) { for(;program[i]!=';';i=ne[i]) { } return i; }
int solve_block(int &L,int R)
{
    if(L>R) return 0;
    string s;
    int t,temp,e,a,b,ttt;
    e=tot_exp++;
    exp_[e].t=block,exp_[e].param.clear();
    for(;L<=R;R=(R>len)?-1:R)
        if(program[L]=='{')
            exp_[e].param.push_back(solve_block(ttt=ne[L],r[L]-1)),L=ne[r[L]];
        else
        {
            s=slice_(L,r[L]);
            if(s=="if")
            {
                temp=tot_exp++;
                exp_[e].param.push_back(temp);
                exp_[temp].t=if_,exp_[temp].param.clear();
                L=ne[r[L]];
                exp_[temp].param.push_back(solve_expression(ne[L],r[L]-1));
                L=ne[r[L]];
                exp_[temp].param.push_back(solve_block(L,len+1));
                if(L<=R && slice_(L,r[L])=="else")
                    L=ne[r[L]],exp_[temp].param.push_back(solve_block(L,len+1));
            }
            else if(s=="while")
            {
                temp=tot_exp++;
                exp_[e].param.push_back(temp);
                exp_[temp].t=while_,exp_[temp].param.clear();
                L=ne[r[L]];
                exp_[temp].param.push_back(solve_expression(ne[L],r[L]-1));
                L=ne[r[L]];
                exp_[temp].param.push_back(solve_block(L,len+1));
            }
            else if(s=="for")
            {
                temp=tot_exp++;
                exp_[e].param.push_back(temp);
                exp_[temp].t=for_,exp_[temp].param.clear();
                L=ne[r[L]];
                t=r[L];
                exp_[temp].param.push_back(solve_statement(ne[L],a=find_semicolon(ne[L])));
                exp_[temp].param.push_back(solve_expression(ne[a],(b=find_semicolon(ne[a]))-1));
                program[t]=';';
                exp_[temp].param.push_back(solve_statement(ne[b],t));
                program[t]=')';
                L=ne[t];
                exp_[temp].param.push_back(solve_block(L,len+1));
            }
            else exp_[e].param.push_back(solve_statement(L,t=find_semicolon(L))),L=ne[t];
        }
    return e;
}
inline void solve_global()
{
    int i,j,l,t,aaa;
    string s;
    vector<int> temp;
    for(i=ne[0];i<=len;i=ne[i])
    {
        i=ne[r[i]];
        s=slice_(i,r[i]);
        assert(s.size());
        i=ne[r[i]];
        if(program[i]=='(')
        {
            t=func_id[s];
            temp.clear();
            for(i=ne[i];program[i]!=')';i=ne[r[i]])
            {
                if(program[i]==',') i=ne[i];
                i=ne[r[i]];
                s=slice_(i,r[i]);
                temp.push_back(var_id[s]);
            }
            func_[t].param=temp;
            i=ne[i];
            func_[t].body=solve_block(aaa=ne[i],r[i]-1);
            i=r[i];
        }
        else
            for(t=var_id[s];;)
            {
                temp.clear();
                for(l=1;program[i]=='[';i=ne[r[i]])
                {
                    t=to_int(slice_(i+1,r[i]-1));
                    if(l>1) temp.push_back(t);
                    l*=t;
                }
                for(j=temp.size()-2;j>=0;j--)
                    temp[j]*=temp[j+1];
                if(l>1) temp.push_back(1);
                var[var_id[s]].push(make_pair(last_p,temp));
                memset(memory+last_p,0,sizeof(int)*l);
                last_p+=l;
                if(program[i]==';') break;
                i=ne[i];
                s=slice_(i,r[i]);
                i=ne[r[i]];
            }
        temp.clear();
    }
}
int main()
{
    read();
    prework();
    solve_global();
    expression run;
    run.t=invoke,run.param.clear(),run.param.push_back(func_id[str("main")]);
    evaluate(run,0);
    return 0;
}
