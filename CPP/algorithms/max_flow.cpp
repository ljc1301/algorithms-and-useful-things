struct edge { int to,cap,rev; };
vector<edge> G[maxn];
bool vis[maxn];
void add_edge(int u,int v,int cap)
{
    G[u].push_back((edge){v,cap,G[v].size()});
    G[v].push_back((edge){u,0,G[u].size()-1});
}
int dfs(int u,int t,int f)
{
    if(u==t) return f;
    vector<edge>::iterator it;
    vis[u]=true;
    for(it=G[u].begin();it!=G[u].end();it++)
        if(!vis[it->to] && it->cap>0)
        {
            int d=dfs(it->to,t,min(f,it->cap));
            if(d>0)
            {
                it->cap-=d;
                G[it->to][it->rev].cap+=d;
                return d;
            }
        }
    return 0;
}
int max_flow(int s,int t)
{
    int res=0;
    for(;;)
    {
        int f;
        memset(vis,0,sizeof(vis));
        f=dfs(s,t,inf);
        if(!f) return res;
        res+=f;
    }
}
