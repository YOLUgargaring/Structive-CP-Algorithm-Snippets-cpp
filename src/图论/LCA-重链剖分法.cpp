#include<bits/stdc++.h>
#define int long long int
#define TCASE 0

using namespace std;

//模板原题: 洛谷P3379 Link: https://www.luogu.com.cn/problem/P3379
struct HLD{//重链剖分
    int n,s;// 点数/根节点
    vector<vector<int>>tr;// 树邻接表
    vector<int>fa;// fa[u]:节点u的父节点
    vector<int>dep;// dep[u]:节点u的深度
    vector<int>siz;// siz[u]:节点u的子树大小
    vector<int>big;// big[u]:节点u的重儿子
    vector<int>top;// top[u]:节点u所在的重链
    vector<int>dfn;// dfn[u]:节点u的dfs序,dfs序重边优先遍历
    vector<int>rnk;// rnk[i]:dfs序对应的节点编号,rnk[dfn[x]]=x
    vector<int>w;// w[u]:节点u的点权
    int tim;// dfs计时器

    HLD(int n,int s):n(n),s(s){
        tr.assign(n+1,{});
        fa.assign(n+1,0);
        dep.assign(n+1,0);
        siz.assign(n+1,0);
        big.assign(n+1,0);
        top.assign(n+1,0);
        dfn.assign(n+1,0);
        rnk.assign(n+1,0);
        w.assign(n+1,0);
        tim=0;
    }

    void addEdge(int u,int v){
        tr[u].push_back(v);
        tr[v].push_back(u);
    }

    void _DFS_FDB(int u,int f){//第一次dfs求出fa[],dep[],siz[],big[]
        fa[u]=f;
        dep[u]=dep[f]+1;
        siz[u]=1;
        for(int v:tr[u]){
            if(v==f) continue;
            _DFS_FDB(v,u);
            siz[u]+=siz[v];
            if(siz[v]>siz[big[u]]) big[u]=v;
        }
    }

    void _DFS_HLD(int u,int ftop){//第二次dfs求出top[],dfn[],rnk[]
        top[u]=ftop;
        dfn[u]=++tim;
        rnk[tim]=u;
        if(big[u]) _DFS_HLD(big[u],ftop);
        for(int v:tr[u]){
            if(v!=big[u]&&v!=fa[u]) _DFS_HLD(v,v);
        }
    }

    void build(){
        _DFS_FDB(s,0);
        _DFS_HLD(s,s);
    }

    int queryLCA(int u,int v){//查询u,v的最近公共祖先,需先调用build()
        while(top[u]!=top[v]){
            if(dep[top[u]]>dep[top[v]]) u=fa[top[u]];
            else v=fa[top[v]];
        }
        return dep[u]>dep[v]?v:u;
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n,m,s;
    cin>>n>>m>>s;
    HLD hld(n,s);
    for(int i=1;i<n;i++){
        int u,v;
        cin>>u>>v;
        hld.addEdge(u,v);
    }
    hld.build();
    while(m--){
        int u,v;
        cin>>u>>v;
        cout<<hld.queryLCA(u,v)<<endl;
    }
} 
/*=============...solving...=============*/

signed main(){
    ios::sync_with_stdio(0);
    cin.tie(nullptr);
    int T=1;
    if(TCASE) cin>>T;
    // init();
	while(T--) solve_by_YOLU();
    return 0;
}
/*Coding by YOLU_gargaring*/