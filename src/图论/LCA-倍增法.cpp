#include<bits/stdc++.h>
#define int long long int
#define endl '\n'
#define pii pair<int,int>

using namespace std;

//模板原题: 洛谷P3379 Link: https://www.luogu.com.cn/problem/P3379
struct LCA_BIN{//倍增法求LCA及其功能扩展,本质运用树上倍增
    constexpr static int J=20;
    int n,s;
    vector<vector<pii>>tr;
    vector<array<int,J>>fa;//节点i的第2^j个祖先
    vector<array<int,J>>mxe;//节点i到其第2^j个祖先路径上的最大边权
    vector<array<int,J>>mne;//节点i到其第2^j个祖先路径上的最小边权
    vector<int>dep;//节点i的深度
    LCA_BIN(int n):n(n),tr(n+1),fa(n+1),mxe(n+1),mne(n+1),dep(n+1,0){
        for(auto&row:fa) row.fill(0);
        for(auto&row:mxe) row.fill(0);
        for(auto&row:mne) row.fill(LLONG_MAX);
    }
    void add(int u,int v,int w){//建树
        tr[u].push_back({v,w});
        tr[v].push_back({u,w});
    }
    void dfs(int rt,int fnd,int fw){//rt为当前节点,fnd为父节点,fw为父节点到当前节点的边权
        fa[rt][0]=fnd;//第一个祖先就是父节点
        dep[rt]=dep[fnd]+1;//深度等于父节点深度加1
        mxe[rt][0]=fw;//第一个祖先路径上的最大边权就是父节点到当前节点的边权
        mne[rt][0]=fw;//第一个祖先路径上的最小边权就是父节点到当前节点的边权
        for(int i=1;i<J;i++){
            //第2^j个祖先等于第2^(j-1)个祖先的第2^(j-1)个祖先
            fa[rt][i]=fa[fa[rt][i-1]][i-1];
            //第2^j个祖先路径上的最大边权等于第2^(j-1)个祖先路径上的最大边权和第2^(j-1)个祖先的第2^(j-1)个祖先路径上的最大边权的最大值
            mxe[rt][i]=max(mxe[rt][i-1],mxe[fa[rt][i-1]][i-1]);
            //第2^j个祖先路径上的最小边权等于第2^(j-1)个祖先路径上的最小边权和第2^(j-1)个祖先的第2^(j-1)个祖先路径上的最小边权的最小值
            mne[rt][i]=min(mne[rt][i-1],mne[fa[rt][i-1]][i-1]);
        }
        for(auto[v,w]:tr[rt]){
            if(v==fnd) continue;
            dfs(v,rt,w);
        }
    }
    void build(int s){//以s为根节点预处理
        this->s=s;
        dfs(s,0,0);
    }
    vector<int> query(int u,int v){//倍增法求u,v的LCA以及u到v路径上的最大/最小边权或其它信息
        int mx=0;
        int mn=LLONG_MAX;
        if(dep[u]<dep[v]) swap(u,v);//选取较深的节点
        int dif=dep[u]-dep[v];
        for(int i=0;i<J;i++){
            if(dif&(1<<i)){
                mx=max(mx,mxe[u][i]);
                mn=min(mn,mne[u][i]);
                u=fa[u][i];
            }
        }
        if(u==v) return {u,mx,mn};//若此时u=v,则u,v均为它们自己的祖先
        for(int i=J-1;i>=0;i--){
            if(fa[u][i]!=fa[v][i]){
                mx=max({mx,mxe[u][i],mxe[v][i]});
                mn=min({mn,mne[u][i],mne[v][i]});
                u=fa[u][i];
                v=fa[v][i];
            }
        }
        mx=max({mx,mxe[u][0],mxe[v][0]});
        mn=min({mn,mne[u][0],mne[v][0]});
        return {fa[u][0],mx,mn};//可根据要求扩展,还可以求路径长度,路径和,路径异或等信息
    }
};

/*=============...solving...=============*/
void solve(){
    int n,m,s;
    cin>>n>>m>>s;//s为树的根节点
    LCA_BIN lca(n);
    for(int i=1;i<n;i++){
        int u,v;
        cin>>u>>v;
        lca.add(u,v,0);
    }
    lca.build(s);
    while(m--){
        int u,v;
        cin>>u>>v;
        auto ans=lca.query(u,v);
        cout<<ans[0]<<endl;
    }
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