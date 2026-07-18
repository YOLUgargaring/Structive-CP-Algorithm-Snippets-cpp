#include<bits/stdc++.h>
#define int long long int

using namespace std;

//模板原题: 洛谷B3647 Link: https://www.luogu.com.cn/problem/B3647
struct Floyd{//Floyd求图中任意两点间的最短路
    int n;//图的节点数
    int INF;//表示两点暂时不可达的无穷大距离
    vector<vector<int>>dis;//dis[u][v]:节点u到节点v的最短距离
    vector<vector<int>>nxt;//nxt[u][v]:当前u到v最短路中,u之后经过的第一个节点

    Floyd(int n):n(n),INF(LLONG_MAX/4),dis(n+1,vector<int>(n+1,LLONG_MAX/4)),nxt(n+1,vector<int>(n+1,-1)){
        for(int i=1;i<=n;i++){
            dis[i][i]=0;
            nxt[i][i]=i;//自己到自己的路径只包含自己
        }
    }

    void addD(int u,int v,int w){//添加一条有向边u->v;若有重边只保留边权最小者
        if(w<dis[u][v]){
            dis[u][v]=w;
            nxt[u][v]=v;//直接边u->v中,u之后经过的第一个节点就是v
        }
    }

    void add(int u,int v,int w){//添加一条无向边u-v
        addD(u,v,w);
        addD(v,u,w);
    }

    void build(){//完成Floyd预处理;调用距离和路径查询接口前必须先调用本函数
        //依次允许节点k作为路径中转点
        //若i->k与k->j均可达,并且经过k更短,就更新i->j的最短路
        for(int k=1;k<=n;k++){
            for(int i=1;i<=n;i++){
                if(dis[i][k]==INF) continue;//i无法到达k,不可能通过k更新其它路径
                for(int j=1;j<=n;j++){
                    if(dis[k][j]==INF) continue;//k无法到达j,两段路径不能拼接
                    if(dis[i][k]+dis[k][j]<dis[i][j]){
                        dis[i][j]=dis[i][k]+dis[k][j];
                        nxt[i][j]=nxt[i][k];
                        //最短路i->j改为i->k->j后,从i出发的第一步应与i->k相同,记录nxt[i][k]
                    }
                }
            }
        }
    }

    int query(int u,int v){//查询u到v的最短距离,需先调用build()
        return dis[u][v];
    }

    bool reachable(int u,int v){//查询u能否到达v,需先调用build()
        return dis[u][v]!=INF;
    }

    vector<int>getPath(int u,int v){//恢复u到v的一条最短路径,需先调用build()
        vector<int>path;
        if(!reachable(u,v)) return path;//不可达时返回空路径
        path.push_back(u);
        while(u!=v){
            u=nxt[u][v];//每次走向当前最短路中的下一节点
            path.push_back(u);
        }
        return path;
    }
};

/*=============...solving...=============*/
void solve(){
    int n,m;
    cin>>n>>m;
    Floyd G(n);
    while(m--){
        int u,v,w;
        cin>>u>>v>>w;
        G.add(u,v,w);
    }
    G.build();
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++) cout<<G.query(i,j)<<" ";
        cout<<endl;
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
