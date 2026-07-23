#include<bits/stdc++.h>
#define int long long int
#define Tcase 0

using namespace std;

//模板原题: 洛谷P3366 Link:https://www.luogu.com.cn/problem/P3366
class Boruvka{
public:
    struct Edge{
        int u,v,w;
    };

private:
    //并查集查询并进行路径压缩
    static int find(int x,vector<int>&parent){
        if(parent[x]==x) return x;
        return parent[x]=find(parent[x],parent);
    }

    //按连通块大小合并;原本不连通时返回true,已经连通时返回false
    static bool unite(int x,int y,vector<int>&parent,vector<int>&size){
        int rx=find(x,parent);
        int ry=find(y,parent);
        if(rx==ry) return false;
        if(size[rx]<size[ry]) swap(rx,ry);
        parent[ry]=rx;
        size[rx]+=size[ry];
        return true;
    }

public:
    //对外接口:使用Boruvka算法求无向图的最小生成树
    //返回值first表示图是否连通,second表示最小生成树边权和
    //调用: Boruvka::build(n,edge)
    static pair<bool,int> build(int n,const vector<Edge>&edge){
        if(n==0) return {false,0};
        vector<int>parent(n+1);
        vector<int>size(n+1,1);
        for(int i=1;i<=n;i++) parent[i]=i;

        int sum=0;
        int cnt=0;
        while(cnt<n-1){
            //best[i]表示本轮开始时,以i为根的连通块选出的最小出边编号
            vector<int>best(n+1,-1);
            for(int i=0;i<(int)edge.size();i++){
                int ru=find(edge[i].u,parent);
                int rv=find(edge[i].v,parent);
                if(ru==rv) continue;
                if(best[ru]==-1||edge[i].w<edge[best[ru]].w) best[ru]=i;
                if(best[rv]==-1||edge[i].w<edge[best[rv]].w) best[rv]=i;
            }

            bool merged=false;
            for(int i=1;i<=n;i++){
                if(find(i,parent)!=i||best[i]==-1) continue;
                Edge e=edge[best[i]];
                if(!unite(e.u,e.v,parent,size)) continue;
                sum+=e.w;
                cnt++;
                merged=true;
            }
            if(!merged) return {false,0};//所有连通块都找不到出边,原图不连通
        }
        return {true,sum};
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n,m;
    cin>>n>>m;
    vector<Boruvka::Edge>edge(m);
    for(int i=0;i<m;i++) cin>>edge[i].u>>edge[i].v>>edge[i].w;
    pair<bool,int>ans=Boruvka::build(n,edge);
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
