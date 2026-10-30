#include<bits/stdc++.h>
#define int long long int
#define TCASE 0

using namespace std;

/*
    模板例题: 此源代码(包括封装结构体模板)解决的是洛谷P4556
    1.线段树合并模板题,求经过一系列树上路径染色操作后(染色后路径节点存下颜色),每个节点存放的数量最多的颜色种类:
        洛谷P4556 Link: https://www.luogu.com.cn/problem/P4556
    2.LCA性质运用拆解路径,分两种线段树共同维护差分信息:
        洛谷P1600 Link: https://www.luogu.com.cn/problem/P1600
*/
struct SegMergeTree{//线段树合并Segment Tree Merge on Tree
    constexpr static int J=20;
    int n;//树的节点数
    int C;//值域最大值
    int maxNode;//最多分配给线段树的节点池数
    int offset;//应对负数下标的偏移量
    vector<vector<int>>tr;
    vector<array<int,J>>fa;//节点i的第2^j个祖先
    vector<int>dep;//节点i的深度
    vector<int>rt;
    int cnt;
    vector<int>ls;
    vector<int>rs;
    // 维护信息↓
    vector<int>mx;//区间最大值差分数组
    vector<int>mxid;//区间最大值的最小编号
    vector<int>ans;//最终对于每个节点的答案

    SegMergeTree(int n,int C,int maxNode,int offset):n(n),C(C),maxNode(maxNode),offset(offset){
        rt.assign(n+5,0);
        tr.assign(n+5,{});
        cnt=0;
        fa.assign(n+5,{});
        dep.assign(n+5,0);
        ls.assign(maxNode+1,0);
        rs.assign(maxNode+1,0);
        // 额外维护信息↓
        mx.assign(maxNode+1,0);
        mxid.assign(maxNode+1,0);
        ans.assign(n+5,0);
    }

    // 树上添加一边u-v
    void addEdge(int u,int v){
        tr[u].push_back(v);
        tr[v].push_back(u);
    }

    void pushup(int p){//部分线段树所需要的pushup操作
        if(mx[ls[p]]<mx[rs[p]]){
            mx[p]=mx[rs[p]];
            mxid[p]=mxid[rs[p]];
        }
        else{
            mx[p]=mx[ls[p]];
            mxid[p]=mxid[ls[p]];
        }
    }

    //单点修改
    void updateP(int &p,int l,int r,int pos,int v){
        if(!p) p=++cnt;
        if(l==r){
            // 更新对应信息
            mx[p]+=v;
            mxid[p]=l;
            return;
        }
        int mid=(l+r)/2;
        if(pos<=mid) updateP(ls[p],l,mid,pos,v);
        else updateP(rs[p],mid+1,r,pos,v);
        pushup(p);
    }

    //单点查询
    int queryP(int p,int l,int r,int pos){
        if(!p) return 0;
        if(l==r){
            // 返回对应位置的值
            
        }
        int mid=(l+r)/2;
        if(pos<=mid) return queryP(ls[p],l,mid,pos);
        else return queryP(rs[p],mid+1,r,pos);
    }

    //合并操作
    int merge(int a,int b,int l,int r){
        if(!a||!b) return a|b;
        if(l==r){
            // 合并信息
            mx[a]+=mx[b];
            mxid[a]=l;
            return a;
        }
        int mid=(l+r)/2;
        ls[a]=merge(ls[a],ls[b],l,mid);
        rs[a]=merge(rs[a],rs[b],mid+1,r);
        pushup(a);
        return a;
    }

    // 初始化构建LCA
    void bulidlca(int u){
        for(int i=1;i<J;i++) fa[u][i]=fa[fa[u][i-1]][i-1];
        for(int v:tr[u]){
            if(v==fa[u][0]) continue;
            dep[v]=dep[u]+1;
            fa[v][0]=u;
            bulidlca(v);
        }
    }

    // 查询u,v两点的LCA
    int queryLCA(int u,int v){
        if(dep[u]<dep[v]) swap(u,v);//选取较深的节点
        int dif=dep[u]-dep[v];
        for(int i=0;i<J;i++){
            if(dif&(1<<i)) u=fa[u][i];
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

    // 向上合并子树线段树维护信息,统计最终所需答案
    void solve(int u){
        for(int v:tr[u]){
            if(v==fa[u][0]) continue;
            solve(v);
            // 合并子树线段树维护信息
            rt[u]=merge(rt[u],rt[v],1,C);
        }
        // 统计最终答案
        ans[u]=mxid[rt[u]];
        if(mx[rt[u]]==0) ans[u]=0;
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    // 每一种救济粮看作一种颜色,考虑树上差分思想,设d[u][c]为节点u上颜色c的差分值
    // 则每个节点都需要维护一个颜色计数的差分数组,最后累加至父节点,但必然MLE
    // 为每个节点维护一棵权值线段树,线段树下标为颜色编号
    // 定义rt[u]为节点u对应的权值线段树根节点,每次救济粮发放将差分操作写入对应节点的线段树,所有操作处理完后再向根节点合并
    // 设两个颜色差分数组A、B,若希望统计两个数组合并的结果只需将对应位置数值相加即可
    // 线段树合并即将两颗权值线段树对应颜色的值逐项相加,对于线段树合并操作,分以下3种情况:
    // 1.若其中一棵线段树为空,则可直接返回非空的树
    // 2.若两树都非空且到达叶子节点,则叶子节点对应的颜色若一样,则直接累加差分计数
    // 3.若两树非空且处于非叶子节点,则分别递归合并左右儿子
    // 所以,倒序dfs序枚举整棵树节点,对应每个节点u,先处理其所有子节点,将每个子节点的线段树合并进rt[u]
    // 此时rt[u]已代表整个子树的差分总和,再查询根节点所维护的最大颜色计数记录答案
    int n,m;
    cin>>n>>m;
    static int C=100000;//颜色种类上限
    auto log2=[&](int x)->int {
        int res=0;
        while((1<<res)<x) res++;
        return res;
    };
    int maxNode=4*m*(log2(C)+1)+5;//线段树节点池分配上限
    SegMergeTree segt(n,C,maxNode,0);
    for(int i=1;i<=n-1;i++){
        int u,v;
        cin>>u>>v;
        segt.addEdge(u,v);
    }
    segt.bulidlca(1);
    for(int i=1;i<=m;i++){
        int x,y,z;
        cin>>x>>y>>z;
        segt.updateP(segt.rt[x],1,C,z,1);
        segt.updateP(segt.rt[y],1,C,z,1);
        int l=segt.queryLCA(x,y);
        segt.updateP(segt.rt[l],1,C,z,-1);
        int lfa=segt.fa[l][0];
        segt.updateP(segt.rt[lfa],1,C,z,-1);
    }
    segt.solve(1);
    for(int i=1;i<=n;i++) cout<<segt.ans[i]<<endl;
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