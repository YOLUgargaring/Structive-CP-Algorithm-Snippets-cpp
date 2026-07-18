#include<bits/stdc++.h>
#define int long long int
#define pii pair<int,int>

using namespace std;

//模板原题: 洛谷P3385 Link: https://www.luogu.com.cn/problem/P3385
struct SPFA{//SPFA求单源最短路并判断从起点可达的负环
    int n;//图的节点数
    int INF;//表示不可达的无穷大距离
    vector<vector<pii>>tu;//tu[u]存储{终点v,边权w}
    vector<int>dis;//dis[u]:起点s到节点u的当前最短距离
    vector<int>cnt;//cnt[u]:当前最短路s->u经过的边数
    vector<bool>inq;//inq[u]:节点u当前是否在队列中

    SPFA(int n):n(n),INF(LLONG_MAX/4),tu(n+1),dis(n+1),cnt(n+1),inq(n+1,false){}

    void add(int u,int v,int w){//向图中添加一条有向边u->v
        tu[u].push_back({v,w});
    }

    bool checkNegCycle(int s){//判断是否存在从起点s可达的负环
        fill(dis.begin(),dis.end(),INF);//fill将dis容器中的所有元素统一赋值为INF
        fill(cnt.begin(),cnt.end(),0);
        fill(inq.begin(),inq.end(),false);
        queue<int>q;
        dis[s]=0;
        q.push(s);
        inq[s]=true;

        while(!q.empty()){
            int u=q.front();
            q.pop();
            inq[u]=false;
            for(auto &[v,w]:tu[u]){
                if(dis[u]+w<dis[v]){//经过u可以继续缩短s->v的距离
                    dis[v]=dis[u]+w;
                    cnt[v]=cnt[u]+1;
                    //n个节点构成的无环简单路径至多经过n-1条边;
                    //若一条不断变短的路径经过至少n条边,其中必然重复经过某个节点,
                    //重复部分就是一个从s可达的负环
                    if(cnt[v]>=n) return true;
                    if(!inq[v]){
                        q.push(v);
                        inq[v]=true;
                    }
                }
            }
        }
        return false;
    }

    int query(int u){//无负环时查询起点到节点u的最短距离,需先调用checkNegCycle(s)
        return dis[u];
    }

    bool reachable(int u){//无负环时查询节点u是否能从起点到达
        return dis[u]!=INF;
    }
};

/*=============...solving...=============*/
void solve(){
    int n,m;
    cin>>n>>m;
    SPFA G(n);
    while(m--){
        int u,v,w;
        cin>>u>>v>>w;
        G.add(u,v,w);
        if(w>=0) G.add(v,u,w);
    }
    if(G.checkNegCycle(1)) cout<<"YES"<<endl;
    else cout<<"NO"<<endl;
}
/*=============...solving...=============*/

signed main(){
    ios::sync_with_stdio(0);
    cin.tie(nullptr);
    int T;
    cin>>T;
    while(T--) solve();
    return 0;
}
/*Coding by YOLU_gargaring*/
