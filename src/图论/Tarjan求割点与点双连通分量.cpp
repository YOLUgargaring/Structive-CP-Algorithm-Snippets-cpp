#include<bits/stdc++.h>
#define int long long int
#define TCASE 0

using namespace std;

/*
    模板例题: 此源代码solve解决的是洛谷P8435
    1.求割点模板题: 洛谷P3388 Link: https://www.luogu.com.cn/problem/P3388
    2.求点双连通分量模板题: 洛谷P8435 Link: https://www.luogu.com.cn/problem/P8435
    3.点双连通分量之间的结构关系,点双与割点的性质: 洛谷P3225 Link :https://www.luogu.com.cn/problem/P3225
*/
struct SCC_VBCC{//Tarjan算法求割点与点双连通分量
    int n,m;//点数/变数
    int tim;//DFS计时器
    vector<vector<pair<int,int>>>tu;//原无向图tu[u]:{v,id}
    vector<int>dfn;//dfn[u]:节点u第一次被DFS访问的时间戳
    vector<int>low;//low[u]:节点u能回溯到的最早栈中节点的时间戳
    int cutcnt;//割点数
    vector<bool>cut;//cut[u]:标记点u是否为割点
    vector<pair<int,int>>stk;//边栈
    int vbcnt;//点双连通分量数
    vector<vector<int>>vbccnd;//点双连通分量列表
    vector<int>vis;//分量编号去重,避免同一点反复加入同一点双连通分量

    SCC_VBCC(int n):n(n),m(0),tim(0),vbcnt(0),cutcnt(0),
                    tu(n+1),dfn(n+1),low(n+1),cut(n+1),vis(n+1){}

    void add(int u,int v){//向图中添加一边u-v
        if(u==v) return;
        m++;
        tu[u].push_back({v,m});
        tu[v].push_back({u,m});
    }

    void Tarjan(int u,int fa){//核心Tarjan算法递归求解割点与边双连通分量,fa为父边编号
        dfn[u]=low[u]=++tim;
        int son=0;//子树数量
        for(auto [v,id]:tu[u]){
            if(!dfn[v]){
                son++;
                stk.push_back({u,v});
                Tarjan(v,id);
                low[u]=min(low[u],low[v]);
                if(low[v]>=dfn[u]){//一个点双连通分量已闭合
                    if(fa!=-1) cut[u]=1;
                    vbcnt++;
                    vbccnd.push_back({});
                    while(1){//持续弹出边栈栈顶边直到树边u-v,这些边构成一个完整的VBCC
                        auto [x,y]=stk.back();
                        stk.pop_back();
                        if(vis[x]!=vbcnt){
                            vis[x]=vbcnt;
                            vbccnd.back().push_back(x);
                        }
                        if(vis[y]!=vbcnt){
                            vis[y]=vbcnt;
                            vbccnd.back().push_back(y);
                        }
                        if(x==u&&y==v) break;
                    }
                }
            }
            else if(id!=fa&&dfn[v]<dfn[u]){//只把指向祖先的回边压栈
                stk.push_back({u,v});
                low[u]=min(low[u],dfn[v]);
            }
        }
        if(fa==-1&&son>=2) cut[u]=1;
    }

    void build(){//求无向图中的割点与点双连通分量
        for(int i=1;i<=n;i++){
            if(!dfn[i]){
                Tarjan(i,-1);
                if(tu[i].empty()){//孤立的单个点也为一个点双连通分量
                    vbcnt++;
                    vbccnd.push_back({i});
                }
            }
        }
    }

    bool isCut(int u){//查询节点u是否为割点,需先调用build()
        return cut[u];
    }

    vector<int> getCuts(){//按节点编号从小到大返回所有割点,需先调用build()
        vector<int>res;
        for(int i=1;i<=n;i++){
            if(cut[i]){
                cutcnt++;
                res.push_back(i);
            }
        }
        return res;
    }

    vector<vector<int>> getAllVBCC(){//查询点双连通分量,需先调用build()
        return vbccnd;
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n,m;
    cin>>n>>m;
    SCC_VBCC G(n);
    while(m--){
        int u,v;
        cin>>u>>v;
        G.add(u,v);
    }
    G.build();
    vector<vector<int>>vbcc=G.getAllVBCC();
    cout<<vbcc.size()<<endl;
    for(auto cc:vbcc){
        cout<<cc.size()<<" ";
        for(int u:cc) cout<<u<<" ";
        cout<<endl;
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