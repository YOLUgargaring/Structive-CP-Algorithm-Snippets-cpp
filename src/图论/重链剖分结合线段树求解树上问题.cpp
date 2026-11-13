#include<bits/stdc++.h>
#define int long long int
#define TCASE 0

using namespace std;

/*
    模板例题: 此源代码solve解决的是洛谷P3384
    1.重链剖分+线段树求解树上路径与子树的点权加与点权和查询: 洛谷P3384 Link: https://www.luogu.com.cn/problem/P3384
    2.重链剖分求解最近公共祖先: 洛谷P3379 Link: https://www.luogu.com.cn/problem/P3379
    3.重链剖分+吉司机线段树求解树上路径点权最大值查询与路径点权和查询与单点节点点权更新:
        洛谷P2590 Link: https://www.luogu.com.cn/problem/P2590
    注: 为了适配不同线段树模板,此封装重链剖分模板只保留了预处理和LCA查询
        solve()函数中有讲解树上路径和子树问题运用重链剖分得到的dfs序结合线段树的处理框架
*/
struct HLD{//重链剖分,求解树上路径与子树问题时常与线段树结合运用
    int n,s;// 点数/根节点
    vector<vector<int>>tr;// 树邻接表
    vector<int>fa;// fa[u]:节点u的父节点
    vector<int>dep;// dep[u]:节点u的深度
    vector<int>siz;// siz[u]:节点u的子树大小,子树包含根节点u自身
    vector<int>big;// big[u]:节点u的重儿子
    vector<int>top;// top[u]:节点u所在的重链
    vector<int>dfn;// dfn[u]:节点u的dfs序,dfs序重边优先遍历
    vector<int>rnk;// rnk[i]:dfs序编号对应的原树节点编号,rnk[dfn[x]]=x
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

    // 第一次dfs求出fa[],dep[],siz[],big[]
    void _DFS_FDB(int u,int f){
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

    // 第二次dfs求出top[],dfn[],rnk[]
    void _DFS_HLD(int u,int ftop){
        top[u]=ftop;
        dfn[u]=++tim;
        rnk[tim]=u;
        if(big[u]) _DFS_HLD(big[u],ftop);
        for(int v:tr[u]){
            if(v!=big[u]&&v!=fa[u]) _DFS_HLD(v,v);
        }
    }

    // 重链剖分,构建重链优先遍历的dfs序
    void build(){
        _DFS_FDB(s,0);
        _DFS_HLD(s,s);
    }

    // 查询u,v的最近公共祖先,需先调用build()
    int queryLCA(int u,int v){
        while(top[u]!=top[v]){
            if(dep[top[u]]>dep[top[v]]) u=fa[top[u]];
            else v=fa[top[v]];
        }
        return dep[u]>dep[v]?v:u;
    }
};

struct SegTree{//懒标记线段树(区间加/乘，查询区间和)
    struct node{
        int data=0;
        int lazy=0;//加值懒标记
        int mlazy=1;//乘值懒标记
    };
    int n,m;//节点数/模数
    vector<node>tree;
    SegTree(int n,int m):n(n),m(m),tree((n+1)<<2){}

    void pushup(int root){//更新当前节点
        tree[root].data=(tree[2*root].data%m+tree[2*root+1].data%m)%m;
    }
    void pushdown(int root,int l,int r){//下放懒标记
        int md=l+(r-l)/2;
        int lenL=md-l+1;
        int lenR=r-md;
        if(tree[root].mlazy!=1){//有区间同乘时优先下放乘值懒标记
            tree[2*root].data=((tree[2*root].data%m)*(tree[root].mlazy%m))%m;
            tree[2*root].lazy=((tree[2*root].lazy%m)*(tree[root].mlazy%m))%m;
            tree[2*root].mlazy=((tree[2*root].mlazy%m)*(tree[root].mlazy%m))%m;
            tree[2*root+1].data=((tree[2*root+1].data%m)*(tree[root].mlazy%m))%m;
            tree[2*root+1].lazy=((tree[2*root+1].lazy%m)*(tree[root].mlazy%m))%m;
            tree[2*root+1].mlazy=((tree[2*root+1].mlazy%m)*(tree[root].mlazy%m))%m;
            tree[root].mlazy=1;
        }
        if(tree[root].lazy!=0){//下放加值懒标记至子节点的值和懒标记
            tree[2*root].data=(tree[2*root].data%m+(tree[root].lazy*lenL)%m)%m;
            tree[2*root].lazy=(tree[2*root].lazy%m+tree[root].lazy%m)%m;
            tree[2*root+1].data=(tree[2*root+1].data%m+(tree[root].lazy*lenR)%m)%m;
            tree[2*root+1].lazy=(tree[2*root+1].lazy%m+tree[root].lazy%m)%m;
            tree[root].lazy=0;//下放后自身懒标记清零
        }
    }

    void build(vector<int>&data,int root,int l,int r){//建树
        if(l==r){
            tree[root].data=data[l]%m;//递归至叶节点终止
            return;
        }
        int md=l+(r-l)/2;
        build(data,2*root,l,md);//左子树
        build(data,2*root+1,md+1,r);//右子树
        pushup(root);
    }
    void build(vector<int>&data){build(data,1,1,n);}//对外接口,无需传入根节点和区间

    void updateP(int root,int l,int r,int p,int v){//单点修改
        if(l==r){
            tree[root].data=v;
            tree[root].lazy=0;
            return;
        }
        int md=l+(r-l)/2;
        pushdown(root,l,r);
        if(p<=md) updateP(2*root,l,md,p,v);
        else updateP(2*root+1,md+1,r,p,v);
        pushup(root);
    }
    void updateP(int p,int v){updateP(1,1,n,p,v);}//对外接口

    void updateAdd(int root,int l,int r,int ul,int ur,int v){//区间元素加同值
        if(ul>r||ur<l) return;
        if(ul<=l&&ur>=r){
            tree[root].data=(tree[root].data%m+(v*(r-l+1))%m)%m;
            tree[root].lazy=(tree[root].lazy%m+v%m)%m;
            return;
        }
        int md=l+(r-l)/2;
        pushdown(root,l,r);
        updateAdd(2*root,l,md,ul,ur,v);
        updateAdd(2*root+1,md+1,r,ul,ur,v);
        pushup(root);
    }
    void updateAdd(int ul,int ur,int v){updateAdd(1,1,n,ul,ur,v);}//对外接口

    void updateMul(int root,int l,int r,int ul,int ur,int v){//区间元素乘同值
        if(ul>r||ur<l) return;
        if(ul<=l&&ur>=r){
            tree[root].data=(tree[root].data%m*v%m)%m;
            tree[root].lazy=(tree[root].lazy%m*v%m)%m;//加值懒标记也需更新
            tree[root].mlazy=(tree[root].mlazy%m*v%m)%m;
            return;
        }
        int md=l+(r-l)/2;
        pushdown(root,l,r);
        updateMul(2*root,l,md,ul,ur,v);
        updateMul(2*root+1,md+1,r,ul,ur,v);
        pushup(root);
    }
    void updateMul(int ul,int ur,int v){updateMul(1,1,n,ul,ur,v);}//对外接口

    int query(int root,int l,int r,int ql,int qr){//查询区间和
        if(ql>r||qr<l) return 0;
        if(ql<=l&&qr>=r) return tree[root].data;//当前区间包含于查询区间，直接返回
        int md=l+(r-l)/2;
        pushdown(root,l,r);
        int lS=query(2*root,l,md,ql,qr)%m;
        int rS=query(2*root+1,md+1,r,ql,qr)%m;
        return (lS+rS)%m;
    }
    int query(int ql,int qr){return query(1,1,n,ql,qr);}//对外接口
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    // 对一棵树进行重链剖分DFS序后,树上的一段路径/某节点的子树都可以映射为DFS序上的一段连续区间
    // 既然路径和子树可对应一段连续区间,则树上路径/子树的修改与查询操作可映射为在一棵线段树上的操作
    // 对于根节点为u的子树,其在DFS序中恰好占据[dfn[u],dfn[u]+siz[u]-1]
    // 则子树所有节点点权加v即为对应区间区间加v,查询子树点权和即为查询对应区间区间和
    // 对于树上路径操作,路径u-v的u,v不一定在同一条重链上,无法直接映射到一个区间
    // 若不处于同一重链,考虑不断把链顶深度更大的点向上跳至链顶父节点,每次跳都处理当前点到链顶的这一段对应区间
    // 对于当前操作路径的端点u,v:
    // 1.若top[u]=top[v],则它们已在同一重链,u,v的路径即为这条重链的dfn[]较小者到较大者的区间,可直接处理
    // 2.否则比较dep[top[u]]和dep[top[v]],把链顶深度更大的点记为要跳的点,设为u(不是就swap),处理区间[dfn[top[u]],dfn[u]]
    // 3.跳至链顶父节点,即u=fa[top[u]],回到第1步继续判断和处理
    int n,m,s,p;
    cin>>n>>m>>s>>p;
    HLD hld(n,s);
    for(int i=1;i<=n;i++) cin>>hld.w[i];
    for(int i=1;i<n;i++){
        int u,v;
        cin>>u>>v;
        hld.addEdge(u,v);
    }
    hld.build();
    vector<int>data(n+1);
    // 以dfs序为下标构建线段树维护数组,data[hld.dfn[i]]=hld.w[i] 等价 data[i]=hld.w[hld.rnk[i]]
    for(int i=1;i<=n;i++) data[hld.dfn[i]]=hld.w[i];
    SegTree seg(n,p);
    seg.build(data);
    auto &fa=hld.fa;
    auto &siz=hld.siz;
    auto &top=hld.top;
    auto &dep=hld.dep;
    auto &dfn=hld.dfn;
    /*
        结合线段树的大致处理框架:
        对于子树问题:
        修改:
        modifySub (int u,int d){
            seg.modify(dfn[u],dfn[u]+siz[u]-1,d);
        }
        查询:
        querySub (int u){
            int res=seg.query(dfn[u],dfn[u]+siz[u]-1);
            return res;
        };
        对于路径问题:
        修改:
        modifyPath (int u,int v,int d){
            while(top[u]!=top[v]){
                if(dep[top[u]]<dep[top[v]]) swap(u,v);
                seg.update(dfn[top[u]],dfn[u],d);
                u=fa[top[u]];
            }
            if(dep[u]>dep[v]) swap(u,v);
            seg.update(dfn[u],dfn[v],d);
        }
        查询:
        queryPath (int u,int v){
            int res=0;
            while(top[u]!=top[v]){
                if(dep[top[u]]<dep[top[v]]) swap(u,v);
                根据区间(dfn[top[u]],dfn[u])的查询结果更新res
                u=fa[top[u]];
            }
            if(dep[u]>dep[v]) swap(u,v);
            根据区间(dfn[u],dfn[v])的查询结果更新res
            return res;
        }
    */
    // 树上路径u-v上的每个点点权加d
    auto pathVertexAdd=[&](int u,int v,int d)->void {
        while(top[u]!=top[v]){
            if(dep[top[u]]<dep[top[v]]) swap(u,v);
            seg.updateAdd(dfn[top[u]],dfn[u],d);
            u=fa[top[u]];
        }
        if(dep[u]>dep[v]) swap(u,v);
        seg.updateAdd(dfn[u],dfn[v],d);
    };
    // 查询树上路径u-v上的点权和
    auto queryPathVertexSum=[&](int u,int v)->int {
        int res=0;
        while(top[u]!=top[v]){
            if(dep[top[u]]<dep[top[v]]) swap(u,v);
            res=(res+seg.query(dfn[top[u]],dfn[u]))%seg.m;
            u=fa[top[u]];
        }
        if(dep[u]>dep[v]) swap(u,v);
        res=(res+seg.query(dfn[u],dfn[v]))%seg.m;
        return res;
    };
    // 以u为根节点的子树中每个点点权加d
    auto subtreeVertexAdd=[&](int u,int d)->void {
        seg.updateAdd(dfn[u],dfn[u]+siz[u]-1,d);
    };
    // 查询以u为根节点的子树的点权和
    auto querySubtreeVertexSum=[&](int u)->int {
        int res=seg.query(dfn[u],dfn[u]+siz[u]-1);
        return res;
    };
    while(m--){
        int op,x,y,z;
        cin>>op;
        if(op==1){
            cin>>x>>y>>z;
            pathVertexAdd(x,y,z);
        }
        else if(op==2){
            cin>>x>>y;
            cout<<queryPathVertexSum(x,y)<<endl;
        }
        else if(op==3){
            cin>>x>>z;
            subtreeVertexAdd(x,z);
        }
        else if(op==4){
            cin>>x;
            cout<<querySubtreeVertexSum(x)<<endl;
        }
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