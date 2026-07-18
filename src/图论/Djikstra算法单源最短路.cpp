#include<bits/stdc++.h>
#define int long long int
#define endl '\n'
#define pii pair<int,int>

using namespace std;

//模板原题: 洛谷P4779 Link: https://www.luogu.com.cn/problem/P4779
struct Dijkstra{//Dijkstra求非负权有向图的单源最短路
    int n;//图的节点数
    int INF;//表示不可达的无穷大距离
    vector<vector<pii>>tu;//tu[u]存储{终点v,边权w}
    vector<int>dis;//dis[u]:起点s到节点u的最短距离
    vector<int>pre;//pre[u]:最短路中节点u的前一个节点,用于恢复路径

    Dijkstra(int n):n(n),INF(LLONG_MAX/4),tu(n+1),dis(n+1),pre(n+1){}

    void add(int u,int v,int w){//向图中添加一条非负权有向边u->v
        tu[u].push_back({v,w});
    }

    void build(int s){//以s为起点求到所有节点的最短路;调用查询接口前必须先调用本函数
        fill(dis.begin(),dis.end(),INF);//fill将dis容器中的所有元素统一赋值为INF
        fill(pre.begin(),pre.end(),-1);
        dis[s]=0;
        //priority_queue默认是大根堆;greater<pii>使其变为小根堆,pii按first优先比较,所以存储{距离,节点}
        priority_queue<pii,vector<pii>,greater<pii>>pq;
        pq.push({0,s});
        while(!pq.empty()){
            auto [d,u]=pq.top();
            pq.pop();
            if(d!=dis[u]) continue;//同一节点可能多次进入堆;若d不是当前最短距离,说明这是过期状态,直接跳过
            for(auto &[v,w]:tu[u]){
                if(dis[u]+w<dis[v]){//经过u可以得到更短的s->v路径
                    dis[v]=dis[u]+w;
                    pre[v]=u;
                    pq.push({dis[v],v});
                }
            }
        }
    }

    int query(int u){//查询起点到节点u的最短距离,需先调用build(s)
        return dis[u];
    }

    bool reachable(int u){//查询节点u是否能从起点到达,需先调用build(s)
        return dis[u]!=INF;
    }

    vector<int>getPath(int t){//恢复起点到终点t的一条最短路径,需先调用build(s)
        vector<int>path;
        if(!reachable(t)) return path;//不可达时返回空路径
        int cur=t;
        while(cur!=-1){
            path.push_back(cur);
            cur=pre[cur];//从终点沿pre不断返回起点
        }
        ranges::reverse(path);//当前顺序为终点到起点,翻转后得到起点到终点
        return path;
    }
};

/*=============...solving...=============*/
void solve(){
    int n,m,s;
    cin>>n>>m>>s;
    Dijkstra G(n);
    for(int i=1;i<=m;i++){
        int u,v,w;
        cin>>u>>v>>w;
        G.add(u,v,w);
    }
    G.build(s);
    for(int i=1;i<=n;i++) cout<<G.query(i)<<" ";
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
