#include<bits/stdc++.h>
#define int long long int
#define endl '\n'
#define pii pair<int,int>
#define all(x) x.begin(),x.end()
#define Tcase 0

using namespace std;

/*
    模板原题: 此源代码的solve()解决的是 洛谷P1967
    1.图上两点路径边权最小值的最大值 洛谷P1967 Link: https://www.luogu.com.cn/problem/P1967
    2.图上两点路径边权最大值的最小值 洛谷P2245 Link: https://www.luogu.com.cn/problem/P2245
    3.图上点对使用最少的前k条边连通 CF1706E Link: https://codeforces.com/problemset/problem/1706/E
    三个题所用的结构体<KRT>和<LCA_BIN>完全相同,其中CF1706E需额外多一个 ST表 或 线段树 进行区间查询
*/
struct KRT{//Kruskal重构树,每次合并时创建一个新节点最为两个连通块根的父节点,此模板常与【LCA倍增法/树上倍增】 搭配使用
    vector<int>fa;//父节点数组,原图中所有点为重构树的叶子节点
    vector<vector<int>>tr;//重构树结构
    vector<int>val;//每次合并所记录的信息
    int tot;

    KRT(int n):fa(2*n+5),tr(2*n+5),val(2*n+5),tot(n){
        for(int i=1;i<=n;i++) fa[i]=i;
    }

    int find(int x){
        if(fa[x]==x) return x;
        else return fa[x]=find(fa[x]);
    }

    void unio(int x,int y,int w){//连通块合并,重构树构建
        int fx=find(x);
        int fy=find(y);
        if(fx==fy) return;
        tot++;
        val[tot]=w;
        fa[tot]=tot;
        fa[fx]=fa[fy]=tot;
        tr[tot].push_back(fx);
        tr[tot].push_back(fy);
    }

    bool chk(int x,int y){//判断x和y是否不属于同一个连通块
        return find(x)^find(y);
    }
};

struct LCA_BIN{//倍增法求LCA,因为KRT可能为森林,和普通模板构建LCA树的方式不同
    constexpr static int J=20;
    int n;
    vector<vector<int>>&tr;
    vector<array<int,J>>fa;//节点i的第2^j个祖先
    vector<int>dep;//节点i的深度

    LCA_BIN(int n,vector<vector<int>>&tree):n(n),tr(tree),fa(n+1),dep(n+1,0){
        for(auto&row:fa) row.fill(0);
    }

    void dfs(int rt){
        for(auto v:tr[rt]){
            fa[v][0]=rt;//第一个祖先就是父节点
            dep[v]=dep[rt]+1;//深度等于父节点深度加1
            for(int i=1;i<J;i++){
                //第2^j个祖先等于第2^(j-1)个祖先的第2^(j-1)个祖先
                fa[v][i]=fa[fa[v][i-1]][i-1];
            }
            dfs(v);
        }
    }

    void build(KRT &krt){//原图不连通时KRT重构结果为森林,需要修改LCA构建逻辑
        for(int i=1;i<=n;i++){
            if(krt.fa[i]==i) dfs(i);
        }
    }

    int query(int u,int v){//倍增法求u,v的LCA
        if(dep[u]<dep[v]) swap(u,v);//选取较深的节点
        int dif=dep[u]-dep[v];
        for(int i=0;i<J;i++){
            if(dif&(1<<i)){
                u=fa[u][i];
            }
        }
        if(u==v) return u;//若此时u=v,则u,v均为它们自己的祖先
        for(int i=J-1;i>=0;i--){
            if(fa[u][i]!=fa[v][i]){
                u=fa[u][i];
                v=fa[v][i];
            }
        }
        return fa[u][0];
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n,m;
    cin>>n>>m;
    vector<array<int,3>>tr(m);
    for(auto &e:tr) cin>>e[0]>>e[1]>>e[2];
    //题意: 求出图中u,v的瓶颈路径,即u<->v的所有路径中,每条路径里边权最小值的最大值
    //考虑将边权从大到小排序后建立Kruskal重构树
    //边权从大到小处理,处理到边权w时,并查集中每个连通块都满足块内任意两点边权不小于w的路径互相到达
    //对于每个查询(u,v),比答案更大的权值阶段,u和v不连通 -> 处理到某个最大权值w时,它们首次属于同一连通块
    //合并u,v所处连通块时,合并对应的重构树节点为u,v的LCA -> 其节点权值w即为u<->v所有路径中边权最小值的最大值
    //即对于每个查询(u,v),ans=krt.val[LCA(u,v)]
    sort(all(tr),[](const array<int,3>&a,const array<int,3>&b){return a[2]>b[2];});//最大生成树
    int q;
    cin>>q;
    if(n==1){
        while(q--) cout<<-1<<endl;
        return;
    }
    KRT krt(n);
    for(auto &e:tr) krt.unio(e[0],e[1],e[2]);
    LCA_BIN lca(krt.tot,krt.tr);
    lca.build(krt);
    while(q--){
        int u,v;
        cin>>u>>v;
        if(krt.chk(u,v)){
            cout<<-1<<endl;
            continue;
        }
        int ans=krt.val[lca.query(u,v)];
        cout<<ans<<endl;
    }
} 
/*=============...solving...=============*/

signed main(){
    ios::sync_with_stdio(0);
    cin.tie(nullptr);
    int T=1;
    if(Tcase) cin>>T;
	while(T--) solve_by_YOLU();
    return 0;
}
/*Coding by YOLU_gargaring*/