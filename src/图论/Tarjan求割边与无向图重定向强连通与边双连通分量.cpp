#include<bits/stdc++.h>
#define int long long int
#define TCASE 0

using namespace std;

/*
    模板例题: 此源代码solve解决的是洛谷P1656
    1.输出割边中边权最小值: HDU4738 Link: https://vjudge.net/problem/HDU-4738
    2.按点编号顺序输出所有割边: 洛谷P1656 Link: https://www.luogu.com.cn/problem/P1656
    3.输出将原无向图重新定向为强连通图的方案: CF118E Link: https://codeforces.com/problemset/problem/118/E
    4.求出图中边双连通分量: 洛谷P8436 Link: https://www.luogu.com.cn/problem/P8436
    5.边双连通分量缩点求无向图最少添加多少边使图中无桥: 洛谷P2860 Link:https://www.luogu.com.cn/problem/P2860
*/
struct SCC_B{//Tarjan求割边(桥)/无向图重定向为强连通图方案/边双连通分量
    int n,m;//点数/边数
    int tim;//DFS计时器
    bool brg;//是否存在桥
    vector<vector<tuple<int,int,int>>>tu;//原无向图tu[u]:{v,w,id}
    vector<int>dfn;//dfn[u]:节点u第一次被DFS访问的时间戳
    vector<int>low;//low[u]:节点u能回溯到的最早栈中节点的时间戳
    vector<tuple<int,int,int>>bridge;//所有割边{u,v,w},每对按小端点在前保存
    vector<bool>isbrg;//isbrg[i]:第i条边是否为桥
    vector<pair<int,int>>reg;//可被重新定向为强连通的方案(u->v边对),存在桥时应不作为方案输出
    vector<int>bcc;//bcc[u]:点u所属边双连通分量
    vector<vector<int>>bccnd;//bccnd[i]:第i个边双连通分量所包含的点
    int cnt;//原无向图连通分量数
    int bcnt;//原无向图边双连通分量数
    vector<int>deg;//边双连通分量缩点后的新图每个点的度数
    vector<vector<int>>ntu;//边双连通分量缩点后的新图,应为森林或树

    SCC_B(int n):n(n),m(0),tim(0),brg(false),cnt(0),bcnt(0),tu(n+1),
                dfn(n+1),low(n+1),bcc(n+1),bccnd(n+1),deg(n+1),ntu(n+1){}

    void add(int u,int v,int w){//图中添加一条无向边u-v并编号
        m++;
        tu[u].push_back({v,w,m});
        tu[v].push_back({u,w,m});
    }

    void Tarjan(int u,int eg){//核心Tarjan算法找桥
        dfn[u]=low[u]=++tim;
        for(auto[v,w,id]:tu[u]){
            if(!dfn[v]){
                reg.push_back({u,v});//树边定向u->v
                Tarjan(v,id);
                low[u]=min(low[u],low[v]);
                if(low[v]>dfn[u]){//判断有桥
                    brg=true;
                    isbrg[id]=true;
                    bridge.push_back({min(u,v),max(u,v),w});
                }
            }
            else if(id!=eg&&dfn[v]<dfn[u]){//回边且v是u的祖先,定向u->v
                reg.push_back({u,v});
                low[u]=min(low[u],dfn[v]);
            }
        }
    }

    void build(){//计算割边和定向方案
        tim=0;
        brg=false;
        fill(dfn.begin(),dfn.end(),0);
        fill(low.begin(),low.end(),0);
        isbrg.assign(m+1,false);
        bridge.clear();
        reg.clear();
        reg.reserve(m);
        for(int i=1;i<=n;i++){
            if(!dfn[i]){
                cnt++;
                Tarjan(i,-1);
            }
        }
    }

    void bccDfs(int u){
        bcc[u]=bcnt;
        for(auto [v,w,id]:tu[u]){
            if(isbrg[id]) continue;//u-v边是桥,v不和u在同一双连通分量
            if(bcc[v]) continue;//v已分配至一双连通分量
            bccDfs(v);
        }
    }

    void buildBCC(){//构造双连通分量,需先调用build()求出图中桥
        for(int i=1;i<=n;i++){
            if(!bcc[i]){
                bcnt++;
                bccDfs(i);
            }
        }
    }

    void rebuild(){//根据边双连通分量缩点,需先调用buildBCC求边双连通分量
        for(int u=1;u<=n;u++){
            for(auto [v,w,id]:tu[u]){
                int bu=bcc[u];
                int bv=bcc[v];
                if(bu!=bv){
                    ntu[bu].push_back(bv);
                    deg[bu]++;
                }
            }
        }
    }

    void allBuild(){//一步到位求割边,边双连通分量,边双连通分量缩点
        build();
        buildBCC();
        rebuild();
    }

    vector<vector<int>> getAllBCC(){
        for(int i=1;i<=n;i++) bccnd[bcc[i]].push_back(i);
        vector<vector<int>>res;
        for(int i=1;i<=bcnt;i++) res.push_back(bccnd[i]);
        return res;
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n,m;
    cin>>n>>m;
    SCC_B G(n);
    while(m--){
        int u,v;
        cin>>u>>v;
        G.add(u,v,1);
    }
    G.build();
    sort(G.bridge.begin(),G.bridge.end());
    for(auto[u,v,w]:G.bridge) cout<<u<<" "<<v<<endl;
} 
/*=============...solving...=============*/

signed main(){
    ios::sync_with_stdio(0);
    cin.tie(nullptr);
    int T=1;
    if(TCASE) cin>>T;
	while(T--) solve_by_YOLU();
    return 0;
}
/*Coding by YOLU_gargaring*/