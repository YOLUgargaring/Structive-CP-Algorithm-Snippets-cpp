/*Coding by YOLU_gargaring*/
/* Codeforces and other OI competition version | Statu of this problem: NaN */
/* Rating of this problem: NaN | CLIST: NaN */
// #pragma GCC optimize("O3,unroll-loops")
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
#define rep0(i,l,r) for(int i=l;i<r;i++)
#define rep1(i,l,r) for(int i=l;i<=r;i++)
#define irep(i,l,r) for(int i=l;i>=r;i--)
#define range(type,name) for(auto&type:name)
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

//The code from Luogu_P3812
struct XOR_Basis{//线性基,用最少的数字表示异或空间
    const static int H=61;
    // const static int H=32;
    vector<int>d;//d[i]:当前线性基中最高位为i的代表元素

    XOR_Basis():d(H){}
    XOR_Basis(int n):d(n){}

    void insert(int x){//将x插入当前线性基
        for(int i=d.size()-1;i>=0;i--){
            if((x>>i)&1){
                if(d[i]==0){
                    d[i]=x;
                    return;
                }
                x^=d[i];
            }
        }
    }

    int queryMax(){//查询当前线性基所能异或出的最大值
        int res=0;
        for(int i=d.size()-1;i>=0;i--) res=max(res,res^d[i]);
        return res;
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n;
    cin>>n;
    XOR_Basis xbs;
    //将序列的所有数插入线性基,线性基会排除一些可以用已有元素异或表示的数
    //直接调用queryMax()即可求出序列中选元素的最大异或和
    rep1(i,1,n){
        int x;
        cin>>x;
        xbs.insert(x);
    }
    cout<<xbs.queryMax();
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