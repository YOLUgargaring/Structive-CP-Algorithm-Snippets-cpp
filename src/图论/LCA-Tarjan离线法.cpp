/*Coding by YOLU_gargaring*/
/* Codeforces and other OI competition version | Statu of this problem: NaN */
/* Rating of this problem: NaN | CLIST: NaN */
#include<bits/stdc++.h>
#define int long long int
#define i32 int32_t
#define i64 int64_t
#define i128 __int128
#define db long double
#define endl '\n'
#define pii pair<int,int>
#define tri tuple<int,int,int>
#define fr first
#define sc second
#define td third
#define prque priority_queue 
#define all(x) x.begin(),x.end()
#define rep(x,n,t) for(int i=x;i<n;i+=t)
#define MX INT_MAX
#define MN INT_MIN
#define E 1e-6
#define st_it set<int>::iterator
#define mst_it multiset<int>::iterator
#define list_it list<int>::iterator
#define itr ::iterator

using namespace std;

struct LCA_TAR{//Tarjan离线法LCA
    int n,m;
    vector<vector<int>>tr;
    vector<vector<pii>>query;//query[u]存{v,id}
    vector<int>ans;
    vector<int>fa;
    vector<bool>vis;//该节点的子树是否全访问过
    LCA_TAR(int n,int m):n(n),m(m),tr(n+1),query(n+1),ans(m+1),fa(n+1),vis(n+1,false){
        for(int i=1;i<=n;i++) fa[i]=i;
    }
    void add(int u,int v){//建树
        tr[u].push_back(v);
        tr[v].push_back(u);
    }
    void queryAdd(int u,int v,int id){//添加询问
        query[u].push_back({v,id});
        query[v].push_back({u,id});
    }
    int find(int x){return fa[x]==x?x:fa[x]=find(fa[x]);}//并查集
    void dfs(int rt,int fnd){
        for(int v:tr[rt]){
            if(v==fnd) continue;
            dfs(v,rt);
            fa[v]=rt;//子树合并至父节点
        }
        vis[rt]=true;
        for(auto[v,id]:query[rt]){//处理和rt相关的所有询问
            if(vis[v]) ans[id]=find(v);
        }
    }
    void build(int s){//以s为根节点离线处理所有询问
        dfs(s,0);
    }
    int getAns(int id){//获取第id个询问的答案
        return ans[id];
    }
};

/*=============...solving...=============*/
void solve(){
    int n,m,s;
    cin>>n>>m>>s;
    LCA_TAR lca(n,m);
    for(int i=1;i<n;i++){
        int u,v;
        cin>>u>>v;
        lca.add(u,v);
    }
    for(int i=1;i<=m;i++){
        int u,v;
        cin>>u>>v;
        lca.queryAdd(u,v,i);
    }
    lca.build(s);
    for(int i=1;i<=m;i++) cout<<lca.getAns(i)<<endl;
}
/*=============...solving...=============*/

signed main(){
    ios::sync_with_stdio(0);
    cin.tie(nullptr);
    int T=1;
    //cin>>T;
    while(T--) solve();
    return 0;
}
/*Coding by YOLU_gargaring*/