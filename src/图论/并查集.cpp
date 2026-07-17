#include<bits/stdc++.h>
#define int long long int
#define Tcase 0

using namespace std;

//模板原题: 洛谷P3367 Link: https://www.luogu.com.cn/problem/P3367
struct DSU{
    int n;
    vector<int>fa;
    vector<int>sz;
    int cnt;
    DSU(int n):n(n),fa(n+1),sz(n+1,1),cnt(n){
        for(int i=1;i<=n;i++) fa[i]=i;
    }
    int find(int x){//查询|路径压缩
        if(fa[x]==x) return x;
        else return fa[x]=find(fa[x]);
    }
    void unio(int x,int y){//按秩合并|连通块合并
        int fx=find(x);
        int fy=find(y);
        if(fx!=fy){
            if(sz[fx]<sz[fy]) swap(fx,fy);
            fa[fy]=fx;
            sz[fx]+=sz[fy];
            cnt--;
        }
    }
    int size(int x){//集合大小|该点所在连通块大小
        return sz[find(x)];
    }
    int chk(int x,int y){//x和y是否不属于同一集合/连通块
        return find(x)^find(y);
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n,m;
    cin>>n>>m;
    DSU dsu(n);
    while(m--){
        int op,x,y;
        cin>>op>>x>>y;
        if(op==1) dsu.unio(x,y);
        else (dsu.find(x)==dsu.find(y))?cout<<"Y"<<endl:cout<<"N"<<endl;
    }
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