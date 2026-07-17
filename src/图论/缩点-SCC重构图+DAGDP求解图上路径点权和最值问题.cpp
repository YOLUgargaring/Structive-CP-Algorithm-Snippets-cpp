#include<bits/stdc++.h>
#define int long long int
#define Tcase 0

using namespace std;

//模板原题: 洛谷P3387 Link: https://www.luogu.com.cn/problem/P3387
struct SCC_S{//Tarjan求强连通分量,缩点后将原图重构为DAG
    int n;//原图节点数
    vector<vector<int>>tu;//原有向图
    vector<int>w;//原图点权
    vector<int>dfn;//dfn[u]:节点u第一次被DFS访问的时间戳
    vector<int>low;//low[u]:节点u能回溯到的最早栈中节点的时间戳
    int tim;//DFS计时器
    stack<int>dst;//存储当前尚未确定所属SCC的节点
    vector<bool>inst;//inst[u]:节点u当前是否在栈dst中
    vector<int>scc;//scc[u]:原图节点u所属的强连通分量编号
    int sccs;//强连通分量总数
    vector<int>sccw;//sccw[i]:第i个强连通分量中所有原节点的点权和
    vector<vector<int>>ntu;//缩点后的新图,强连通分量作为节点,一定是DAG
    vector<int>in;//缩点后各节点的入度,用于拓扑排序
    vector<int>dp;//dp[i]:DAG中以第i个SCC结尾的路径最大点权和

    SCC_S(int n):n(n),tu(n+1),w(n+1),dfn(n+1),low(n+1),tim(0),inst(n+1,false),
               scc(n+1),sccs(0),sccw(n+1),ntu(n+1),in(n+1),dp(n+1){}

    void setW(int u,int v){//设置原图节点u的点权为v
        w[u]=v;
    }

    void add(int u,int v){//向原有向图中添加一条u->v的边
        tu[u].push_back(v);
    }

    void Tarjan(int u){//从节点u开始DFS,求出其所在的强连通分量
        dfn[u]=low[u]=++tim;
        dst.push(u);
        inst[u]=true;
        for(int v:tu[u]){
            if(!dfn[v]){//v未被访问,继续向下DFS
                Tarjan(v);
                low[u]=min(low[u],low[v]);
            }
            else if(inst[v]){
                //v已访问且仍在栈中,说明u可以沿返祖边回到v
                low[u]=min(low[u],dfn[v]);
            }
        }
        if(dfn[u]==low[u]){//u是当前强连通分量在DFS树中的根
            sccs++;
            while(1){
                int x=dst.top();
                dst.pop();
                inst[x]=false;
                scc[x]=sccs;
                sccw[sccs]+=w[x];//同一SCC缩成一个点,点权为内部原节点点权和
                if(x==u) break;
            }
        }
    }

    void rebuild(){//根据scc编号将原图缩点并重构为DAG
        for(int u=1;u<=n;u++){
            for(int v:tu[u]){
                int su=scc[u];
                int sv=scc[v];
                if(su!=sv){//同一SCC内部的边在缩点后不需要保留
                    ntu[su].push_back(sv);
                    in[sv]++;
                }
            }
        }
        //不同原边可能形成重复的SCC边,保留重复边不影响拓扑排序和DP正确性
    }

    void build(){//对整张图求SCC并完成缩点;调用DAGDP前必须先调用本函数
        for(int i=1;i<=n;i++){
            if(!dfn[i]) Tarjan(i);//原图可能不连通,每个未访问节点都要作为DFS起点
        }
        rebuild();
    }

    int DAGDP(){//在缩点后的DAG上进行拓扑DP,返回路径点权和最大值
        queue<int>q;
        vector<int>deg=in;//复制入度,避免拓扑排序破坏成员in,使本函数可以重复调用
        for(int i=1;i<=sccs;i++){
            dp[i]=sccw[i];//一条路径可以直接从当前SCC开始
            if(deg[i]==0) q.push(i);
        }
        while(!q.empty()){
            int u=q.front();
            q.pop();
            for(int v:ntu[u]){
                dp[v]=max(dp[v],dp[u]+sccw[v]);
                deg[v]--;
                if(deg[v]==0) q.push(v);
            }
        }
        int ans=0;
        for(int i=1;i<=sccs;i++) ans=max(ans,dp[i]);
        return ans;
    }

    int getSCC(int u){//查询原图节点u所属的强连通分量编号,需先调用build()
        return scc[u];
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n,m;
    cin>>n>>m;
    SCC_S G(n);
    for(int i=1;i<=n;i++){
        int w;
        cin>>w;
        G.setW(i,w);
    }
    for(int i=1;i<=m;i++){
        int u,v;
        cin>>u>>v;
        G.add(u,v);
    }
    G.build();//求SCC并缩点
    int ans=G.DAGDP();//在缩点后的DAG上求最大路径点权和
    cout<<ans;
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