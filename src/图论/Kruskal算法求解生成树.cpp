#include<bits/stdc++.h>
#define int long long int
#define Tcase 0

using namespace std;

//模板原题: 洛谷P3366 Link:https://www.luogu.com.cn/problem/P3366
class Kruskal{
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
    //对外接口:使用Kruskal算法求无向图的最小或最大生成树
    //minimum=true时求最小生成树,minimum=false时求最大生成树
    //返回值first表示图是否连通,second表示生成树边权和
    //最小生成树调用:Kruskal::build(n,edge)
    //最大生成树调用:Kruskal::build(n,edge,false)
    static pair<bool,int> build(int n,vector<Edge>edge,bool minimum=true){
        if(n==0) return {false,0};
        //参数edge按值传入,因此内部排序不会改变调用者保存的原边集
        //[minimum]表示让排序函数读取minimum,从而切换边权升序或降序
        sort(edge.begin(),edge.end(),[minimum](const Edge&a,const Edge&b){
            if(minimum) return a.w<b.w;
            return a.w>b.w;
        });
        vector<int>parent(n+1);
        vector<int>size(n+1,1);
        for(int i=1;i<=n;i++) parent[i]=i;
        int sum=0;
        int cnt=0;
        for(int i=0;i<(int)edge.size();i++){
            if(!unite(edge[i].u,edge[i].v,parent,size)) continue;
            sum+=edge[i].w;
            cnt++;
            if(cnt==n-1) break;
        }
        return {cnt==n-1,sum};
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n,m;
    cin>>n>>m;
    vector<Kruskal::Edge>edge(m);
    for(int i=0;i<m;i++) cin>>edge[i].u>>edge[i].v>>edge[i].w;
    pair<bool,int>ans=Kruskal::build(n,edge);
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