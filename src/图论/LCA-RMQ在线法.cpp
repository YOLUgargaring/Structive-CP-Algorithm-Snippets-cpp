#include<bits/stdc++.h>
#define int long long int
#define MX INT_MAX
#define MN INT_MIN

using namespace std;

//模板原题: 洛谷P3379 Link: https://www.luogu.com.cn/problem/P3379
struct LCA_RMQ{//欧拉序RMQ法LCA
    constexpr static int J=21;//logN
    int n;
    vector<vector<int>>tr;
    vector<int>dfn;//欧拉序
    int tot;
    vector<int>dep;
    vector<int>pos;//每个节点首次出现的位置
    vector<array<int,J>>ST;
    vector<int>logn;
    LCA_RMQ(int n):n(n),tr(n+1),dfn(2*n),tot(0),dep(2*n),pos(n+1),ST(2*n),logn(2*n+1){}
    void add(int u,int v){//建树
        tr[u].push_back(v);
        tr[v].push_back(u);
    }
    int mindep(int p1,int p2){//比较两位置深度,返回较浅者的位置
        return dep[p1]<dep[p2]?p1:p2;
    }
    void dfs(int u,int fa,int d){
        pos[u]=tot;
        dfn[tot]=u;
        dep[tot]=d;
        tot++;
        for(int v:tr[u]){
            if(v==fa) continue;
            dfs(v,u,d+1);
            dfn[tot]=u;
            dep[tot]=d;
            tot++;
        }
    }
    void STbuild(){//构建稀疏表
        logn[1]=0;
        logn[2]=1;
        for(int i=3;i<=tot;i++) logn[i]=logn[i/2]+1;
        for(int i=0;i<=tot;i++) ST[i][0]=i;
        for(int j=1;(1<<j)<=tot;j++){
            for(int i=0;i+(1<<j)-1<tot;i++){
                ST[i][j]=mindep(ST[i][j-1],ST[i+(1<<(j-1))][j-1]);
            }
        }
    }
    int rmq(int l,int r){//RMQ查询区间深度最小值的位置
        int k=logn[r-l+1];
        return mindep(ST[l][k],ST[r-(1<<k)+1][k]);
    }
    void build(int s){//以s为根节点预处理
        dfs(s,0,1);
        STbuild();
    }
    int query(int u,int v){//欧拉序RMQ法求u,v的LCA
        int l=pos[u];
        int r=pos[v];
        if(l>r) swap(l,r);
        int p=rmq(l,r);
        return dfn[p];
    }
};

/*=============...solving...=============*/
void solve(){
    int n,m,s;
    cin>>n>>m>>s;//s为树的根节点
    LCA_RMQ lca(n);
    for(int i=1;i<n;i++){
        int u,v;
        cin>>u>>v;
        lca.add(u,v);
    }
    lca.build(s);
    while(m--){
        int u,v;
        cin>>u>>v;
        cout<<lca.query(u,v)<<endl;
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