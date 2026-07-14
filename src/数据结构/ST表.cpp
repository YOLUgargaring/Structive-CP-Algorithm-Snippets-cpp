/*Coding by YOLU_gargaring*/
/* Codeforces and other OI competition version | Statu of this problem: NaN */
/* Rating of this problem: NaN | CLIST: NaN */
#pragma GCC optimize("O3,unroll-loops")
// #pragma GCC target("avx2")
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
#define vecin(type,name,n) vector<type>name(n);for(auto&x:name) cin>>x
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

struct ST{
    const static int M=200001;
	const static int lgN=21;
    int n;
    vector<int>logn;
	vector<vector<int>>mx;//区间最大值
	vector<vector<int>>mn;//区间最小值
    ST(int n): n(n),logn(M),mx(n+1,vector<int>(lgN+1,0)),mn(n+1,vector<int>(lgN+1,0)){}
    void pre(){
        logn[1]=0;
        logn[2]=1;
        for(int i=3;i<M;i++) logn[i]=logn[i/2]+1;
    }//logn预处理
    void init(vector<int>&nums){
        pre();
        for(int i=1;i<=n;i++){
            mx[i][0]=mn[i][0]=nums[i-1];
        }
        for(int j=1;j<=lgN;j++){
            for(int i=1;i+(1<<j)-1<=n;i++){
                mx[i][j]=max(mx[i][j-1],mx[i+(1<<(j-1))][j-1]);
                mn[i][j]=min(mn[i][j-1],mn[i+(1<<(j-1))][j-1]);
            }
        }
    }//造表,1-based	
    int queryMax(int l,int r){//区间max,1-based
        int s=logn[r-l+1];
		int maxlr=max(mx[l][s],mx[r-(1<<s)+1][s]);
        return maxlr;
    }
    int queryMin(int l,int r){//区间min,1-based
        int s=logn[r-l+1];
        int minlr=min(mn[l][s],mn[r-(1<<s)+1][s]);
        return minlr;
    }
    int queryMinMaxDiff(int l,int r){//区间max-min,1-based
        return queryMax(l,r)-queryMin(l,r);
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n,m;
    cin>>n>>m;
    vecin(int,a,n);
    ST st(n);
    st.init(a);
    while(m--){
        int l,r;
        cin>>l>>r;
        cout<<st.queryMax(l,r)<<endl;
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