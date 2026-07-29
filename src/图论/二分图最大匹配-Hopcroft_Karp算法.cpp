#include<bits/stdc++.h>
#define int long long int
#define Tcase 0

using namespace std;

//模板原题: Library Checker-Matching on Bipartite Graph | Link:https://judge.yosupo.jp/problem/bipartitematching
class HopKarp{//Hopcroft-Karp算法二分图最大匹配
private:
    int L;//左边点个数
    int R;//右边点个数
    vector<vector<int>>tu;//邻接表图
    vector<int>Lmatch;//左边点所匹配的右边点
    vector<int>Rmatch;//右边点所匹配的左边点
    vector<int>dist;//分层图中左边点到起点的距离
    int matchCount;//匹配边数
    vector<array<int,2>>matchResult;//匹配结果
    const int INF=LLONG_MAX/2;

    bool bfs(){//BFS建立增广路分层图
        queue<int>q;
        for(int i=1;i<=L;i++){
            if(!Lmatch[i]){
                dist[i]=0;
                q.push(i);
            }
            else dist[i]=INF;
        }
        dist[0]=INF;
        while(!q.empty()){
            int u=q.front();
            q.pop();
            if(dist[u]>=dist[0]) continue;
            for(const auto &v:tu[u]){
                int last=Rmatch[v];
                if(dist[last]!=INF) continue;
                dist[last]=dist[u]+1;
                q.push(last);
            }
        }
        return dist[0]!=INF;
    }

public:
    HopKarp(int n,int m):L(n),R(m),matchCount(0){
        tu.assign(n+1,{});
        Lmatch.assign(n+1,0);
        Rmatch.assign(m+1,0);
        dist.assign(n+1,INF);
    }

    void addEdge(int u,int v){//加入边
        tu[u].push_back(v);
    }

    bool dfs(int u){//DFS在分层图中寻找增广路
        if(!u) return true;
        for(const auto &v:tu[u]){
            int last=Rmatch[v];
            if(dist[last]!=dist[u]+1) continue;
            if(dfs(last)){
                Lmatch[u]=v;
                Rmatch[v]=u;
                return true;
            }
        }
        dist[u]=INF;
        return false;
    }

    void Macth(){//二分图最大匹配,查询前需调用此接口
        while(bfs()){
            for(int i=1;i<=L;i++){
                if(!Lmatch[i]&&dfs(i)) matchCount++;
            }
        }
        matchResult.clear();
        for(int i=1;i<=L;i++){
            if(Lmatch[i]) matchResult.push_back({i,Lmatch[i]});
        }
    }

    int queryMaxMatch(){//查询二分图最大匹配的边数
        return matchCount;
    }

    vector<array<int,2>> queryMatchInfo(){//查询具体匹配的边
        return matchResult;
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int l,r,m;
    cin>>l>>r>>m;
    HopKarp HK(l,r);
    while(m--){
        int u,v;
        cin>>u>>v;
        u++,v++;
        HK.addEdge(u,v);
    }
    HK.Macth();
    cout<<HK.queryMaxMatch()<<endl;
    auto ans=HK.queryMatchInfo();
    for(const auto &[l,r]:ans) cout<<l-1<<" "<<r-1<<endl;
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
