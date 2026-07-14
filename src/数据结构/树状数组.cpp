/*Coding by YOLU_gargaring*/
/* Codeforces and other OI competition version | Statu of this problem: NaN */
/* Rating of this problem: NaN | CLIST: NaN */
#include<bits/stdc++.h>
#define int long long int
#define i32 int32_t
#define i64 int64_t
#define i128 __int128
#define db long double
#define endl '\n'
#define pii pair<int,int>
#define tri tuple<int,int,int>
#define fr first
#define sc second
#define td third
#define prque priority_queue 
#define unmap unordered_map
#define unset unordered_set
#define all(x) x.begin(),x.end()
#define rep(x,n,t) for(int i=x;i<n;i+=t)
#define MX INT_MAX
#define MN INT_MIN
#define E 1e-6
#define st_it set<int>::iterator
#define mst_it multiset<int>::iterator
#define list_it list<int>::iterator
#define itr ::iterator
 
using namespace std;

struct FwTree{
    const static int lowbit(int x){return x&(-x);}
    int n;
    vector<int>tree;
    FwTree(int n):n(n),tree(n+1,0){}
    void update(int p,int d){
        for(int i=p;i<=n;i+=lowbit(i)) tree[i]+=d;
    }
    int query(int p){
        int s=0;
        for(int i=p;i>0;i-=lowbit(i)) s+=tree[i];
        return s;
    }
    int query(int l,int r){
        if(l>r) return 0;
        return query(r)-query(l-1);
    }
};

/*=============...solving...=============*/
void solve(){
    int n,m;
    cin>>n>>m;
    FwTree T(n);
    vector<int>a(n+1);
    for(int i=1;i<=n;i++){
        cin>>a[i];
        T.update(i,a[i]);
    }
    while(m--){
        int op;
        cin>>op;
        if(op==1){
            int p,x;
            cin>>p>>x;
            T.update(p,x);
        }
        else{
            int l,r;
            cin>>l>>r;
            cout<<T.query(l,r)<<endl;
        }
    }
    
} 
/*=============...solving...=============*/

signed main(){
    ios::sync_with_stdio(0);
    cin.tie(nullptr);
    int T=1;
    // cin>>T;
	while(T--) solve();
    return 0;
}
/*Coding by YOLU_gargaring*/