#include<bits/stdc++.h>
#define int long long int
#define TCASE 0

using namespace std;

/*
    模板例题: 此源代码solve解决的是HDU4738
    1.输出割边中边权最小值: HDU4738 Link: https://vjudge.net/problem/HDU-4738
    2.按点编号顺序输出所有割边: 洛谷P1656 Link: https://www.luogu.com.cn/problem/P1656
    3.输出将原无向图重新定向为强连通图的方案: CF118E Link: https://codeforces.com/problemset/problem/118/E
*/
struct SCC_B{//Tarjan求割边及无向图重定向为强连通图方案
    int n,m;//点数/边数
    int tim;//DFS计时器
    bool brg;//是否存在桥
    vector<vector<tuple<int,int,int>>>tu;//原无向图tu[u]:{v,w,id}
    vector<int>dfn;//dfn[u]:节点u第一次被DFS访问的时间戳
    vector<int>low;//low[u]:节点u能回溯到的最早栈中节点的时间戳
    vector<tuple<int,int,int>>bridge;//所有割边{u,v,w},每对按小端点在前保存
    vector<pair<int,int>>reg;//可被重新定向为强连通的方案(u->v边对),存在桥时应不作为方案输出
    int cnt;//原无向图连通分量数

    SCC_B(int n):n(n),m(0),tim(0),brg(false),cnt(0),tu(n+1),dfn(n+1),low(n+1){}

    void add(int u,int v,int w){
        m++;
        tu[u].push_back({v,w,m});
        tu[v].push_back({u,w,m});
    }

    void Tarjan(int u,int eg){
        dfn[u]=low[u]=++tim;
        for(auto[v,w,id]:tu[u]){
            if(!dfn[v]){
                reg.push_back({u,v});//树边定向u->v
                Tarjan(v,id);
                low[u]=min(low[u],low[v]);
                if(low[v]>dfn[u]){//判断有桥
                    brg=true;
                    bridge.push_back({min(u,v),max(u,v),w});
                }
            }
            else if(id!=eg&&dfn[v]<dfn[u]){//回边且v是u的祖先,定向u->v
                reg.push_back({u,v});
                low[u]=min(low[u],dfn[v]);
            }
        }
    }

    void build(){//初始或重新计算割边和定向方案
        tim=0;
        brg=false;
        fill(dfn.begin(),dfn.end(),0);
        fill(low.begin(),low.end(),0);
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
};

int n,m;

/*=============...solving...=============*/
void solve_by_YOLU(){
    SCC_B G(n);
    while(m--){
        int u,v,w;
        cin>>u>>v>>w;
        G.add(u,v,w);
    }
    G.build();
    if(G.cnt>1){//本身不连通就不用炸
        cout<<0<<endl;
        return;
    }
    if(!G.brg){
        cout<<-1<<endl;
        return;
    }
    int ans=INT_MAX;
    for(auto[u,v,w]:G.bridge) ans=min(ans,w);
    if(ans==0) ans=1;//至少一人去炸桥
    cout<<ans<<endl;
} 
/*=============...solving...=============*/

signed main(){
    ios::sync_with_stdio(0);
    cin.tie(nullptr);
    int T=1;
    if(TCASE) cin>>T;
	while(cin>>n>>m){
        if(n==0&&m==0) break;
        solve_by_YOLU();
    }
    return 0;
}
/*Coding by YOLU_gargaring*/