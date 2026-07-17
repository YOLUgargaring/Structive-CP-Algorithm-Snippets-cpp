#include<bits/stdc++.h>
#define int long long int
#define endl '\n'
#define pii pair<int,int>
#define Tcase 0

using namespace std;

//模板原题: 洛谷3388 Link: https://www.luogu.com.cn/problem/P3388
struct SCC_C{//Tarjan求无向图割点
    int n;//图的节点数
    int edgeCnt;//边编号计数器,同一条无向边的两个方向使用相同编号
    vector<vector<pii>>tu;//tu[u]存储{相邻节点,边编号}
    vector<int>dfn;//dfn[u]:节点u第一次被DFS访问的时间戳
    vector<int>low;//low[u]:节点u不经过父边能到达的最早节点时间戳
    int tim;//DFS计时器
    vector<bool>cut;//cut[u]:节点u是否为割点
    vector<int>fa;//fa[u]:节点u在DFS树中的父节点
    vector<int>fedge;//fedge[u]:父节点进入u时经过的边编号
    vector<int>chd;//chd[u]:节点u在DFS树中的子树数量
    vector<int>cur;//cur[u]:节点u下一条待访问邻接边的位置

    SCC_C(int n):n(n),edgeCnt(0),tu(n+1),dfn(n+1),low(n+1),tim(0),cut(n+1,false),
                    fa(n+1),fedge(n+1),chd(n+1),cur(n+1){}

    void add(int u,int v){//添加一条无向边u-v
        edgeCnt++;
        tu[u].push_back({v,edgeCnt});
        tu[v].push_back({u,edgeCnt});
    }

    void Tarjan(int s){//从节点s开始进行非递归Tarjan,处理其所在连通块
        stack<int>stk;//显式栈,避免图退化成链时递归层数过深造成系统栈溢出
        dfn[s]=low[s]=++tim;
        fa[s]=fedge[s]=0;//s是当前DFS树的根,没有父节点和父边
        stk.push(s);

        while(!stk.empty()){
            int u=stk.top();
            if(cur[u]<(int)tu[u].size()){//节点u仍有邻接边尚未访问
                pii e=tu[u][cur[u]];
                cur[u]++;
                int v=e.first;
                int id=e.second;
                if(!dfn[v]){//v尚未访问,边u-v成为DFS树边
                    fa[v]=u;
                    fedge[v]=id;
                    chd[u]++;
                    dfn[v]=low[v]=++tim;
                    stk.push(v);
                }
                else if(id!=fedge[u]){
                    //只跳过进入u时使用的父边;若u与父节点间有重边,其它边仍是返祖边
                    low[u]=min(low[u],dfn[v]);
                }
            }
            else{//u的所有邻接边均已处理,模拟递归函数返回父节点
                stk.pop();
                if(fa[u]==0){
                    //DFS树根没有父节点,只有存在至少两棵子树时才是割点
                    if(chd[u]>=2) cut[u]=true;
                }
                else{
                    int f=fa[u];
                    low[f]=min(low[f],low[u]);//子节点u回溯后更新父节点f的low
                    //非DFS树根节点f是割点的条件:
                    //u无法通过返祖边到达f的祖先,删除f后u的子树会与祖先部分断开
                    if(fa[f]!=0&&low[u]>=dfn[f]) cut[f]=true;
                }
            }
        }
    }

    void build(){//求整张无向图的所有割点;调用查询接口前必须先调用本函数
        for(int i=1;i<=n;i++){
            if(!dfn[i]) Tarjan(i);//原图可能不连通,每个连通块都要进行一次DFS
        }
    }

    bool isCut(int u){//查询节点u是否为割点,需先调用build()
        return cut[u];
    }

    vector<int>getCuts(){//按节点编号从小到大返回所有割点
        vector<int>res;
        for(int i=1;i<=n;i++){
            if(cut[i]) res.push_back(i);
        }
        return res;
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n,m;
    cin>>n>>m;
    SCC_C G(n);
    for(int i=1;i<=m;i++){
        int u,v;
        cin>>u>>v;
        G.add(u,v);
    }
    G.build();//处理原图并预处理所有割点
    vector<int>ans=G.getCuts();//获取所有割点
    cout<<ans.size()<<endl;
    for(int x:ans) cout<<x<<" ";
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