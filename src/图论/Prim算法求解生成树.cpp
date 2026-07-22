#include<bits/stdc++.h>
#define int long long int
#define Tcase 0

using namespace std;

//模板原题: 洛谷P3366 Link:https://www.luogu.com.cn/problem/P3366
class Prim{
public:
    struct Edge{
        int u,v,w;
    };

    //对外接口:使用Prim算法求无向图的最小或最大生成树
    //minimum=true时求最小生成树,minimum=false时求最大生成树
    //返回值first表示图是否连通,second表示生成树边权和
    //最小生成树调用:Prim::build(n,edge)
    //最大生成树调用:Prim::build(n,edge,false)
    static pair<bool,int> build(int n,const vector<Edge>&edge,bool minimum=true,int start=1){
        vector<vector<pair<int,int>>>G(n+1);
        for(int i=0;i<(int)edge.size();i++){
            int u=edge[i].u;
            int v=edge[i].v;
            int w=edge[i].w;
            G[u].push_back({v,w});
            G[v].push_back({u,w});
        }
        if(n==0||start<1||start>n) return {false,0};
        vector<bool>vis(n+1,false);
        //优先队列默认取最大值;存入-w可使其优先取最小边权
        //求最大生成树时直接存入w,因此同一份逻辑可以兼容两种生成树
        priority_queue<pair<int,int>>q;
        q.push({0,start});
        int sum=0;
        int cnt=0;
        while(!q.empty()){
            int priority=q.top().first;
            int u=q.top().second;
            q.pop();
            if(vis[u]) continue;
            vis[u]=true;
            cnt++;
            sum+=minimum?-priority:priority;
            for(int i=0;i<(int)G[u].size();i++){
                int v=G[u][i].first;
                int w=G[u][i].second;
                if(vis[v]) continue;
                q.push({minimum?-w:w,v});
            }
        }
        return {cnt==n,sum};
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n,m;
    cin>>n>>m;
    vector<Prim::Edge>edge(m);
    for(int i=0;i<m;i++) cin>>edge[i].u>>edge[i].v>>edge[i].w;
    pair<bool,int>ans=Prim::build(n,edge);
    if(!ans.first) cout<<"orz"<<endl;
    else cout<<ans.second<<endl;
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