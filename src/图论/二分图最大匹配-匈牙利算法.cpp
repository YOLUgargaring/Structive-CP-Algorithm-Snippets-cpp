#include<bits/stdc++.h>
#define int long long int
#define Tcase 0

using namespace std;

//模板原题: 洛谷P2756 Link:https://www.luogu.com.cn/problem/P2756
class Hungary{//匈牙利算法二分图匹配
private:
    int L;//左边点个数
    int R;//右边点个数
    vector<vector<int>>tu;//邻接表图
    vector<int>Lmatch;//左边点所匹配的右边点
    vector<int>Rmatch;//右边点所匹配的左边点
    vector<bool>vis;
    int matchCount;//匹配边数
    vector<array<int,2>>matchResult;//匹配结果

public:
    Hungary(int n,int m):L(n),R(m),matchCount(0){
        tu.assign(n+1,{});
        Lmatch.assign(n+1,0);
        Rmatch.assign(m+1,0);
        vis.assign(m+1,false);
    }

    void addEdge(int u,int v){//加入边
        tu[u].push_back(v);
    }

    bool dfs(int u){//DFS匹配
        for(const auto &v:tu[u]){
            if(vis[v]) continue;
            vis[v]=true;
            if(!Rmatch[v]||dfs(Rmatch[v])){
                Rmatch[v]=u;
                Lmatch[u]=v;
                return true;
            }
        }
        return false;
    }

    void Macth(){//二分图最大匹配,查询前需调用此接口
        for(int i=1;i<=L;i++){
            fill(vis.begin(),vis.end(),false);
            if(dfs(i)) matchCount++;
        }
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
    int m,n;
    cin>>m>>n;
    Hungary hun(m,n);
    while(1){
        int u,v;
        cin>>u>>v;
        if(u==-1) break;
        hun.addEdge(u,v);
    }
    hun.Macth();
    cout<<hun.queryMaxMatch()<<endl;
    auto ans=hun.queryMatchInfo();
    for(auto [l,r]:ans) cout<<l<<" "<<r<<endl;
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