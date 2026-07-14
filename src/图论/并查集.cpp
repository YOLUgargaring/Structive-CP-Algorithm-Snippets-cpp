/*Coding by YOLU_gargaring*/
/* Codeforces and other OI competition version | Statu of this problem: NaN */
/* Rating of this problem: NaN | CLIST: NaN */
#pragma GCC optimize("O3,unroll-loops")
#include<bits/stdc++.h>
#define int long long int
#define i32 int32_t
#define i64 int64_t
#define i128 __int128
#define ust unsigned short int
#define db long double
#define endl '\n'
#define pii pair<int,int>
#define tri tuple<int,int,int>
#define fr first
#define sc second
#define prque priority_queue 
#define unmap unordered_map
#define unset unordered_set
#define all(x) x.begin(),x.end()
#define rep(x,n,t) for(int i=x;i<n;i+=t)
#define vecin(name,n) vector<int>name(n);for(auto&x:name) cin>>x
#define vecout(name) for(auto&x:name) cout<<x<<" ";cout<<endl;
#define coutfix(x) fixed<<setprecision(x)
#define st_it set<int>::iterator
#define mst_it multiset<int>::iterator
#define list_it list<int>::iterator
#define itr ::iterator
#define MX INT_MAX
#define MN INT_MIN
#define E 1e-6
#define BIG 1e18
#define mod (int)(998244353)
// #define mod (int)(1e9+7)
#define Tcase 0

using namespace std;

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
        }
        cnt--;
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