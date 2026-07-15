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
#define coutfix(x) fixed<<setprecision(x)
#define MX INT_MAX
#define MN INT_MIN
#define E 1e-6
#define st_it set<int>::iterator
#define mst_it multiset<int>::iterator
#define list_it list<int>::iterator
#define itr ::iterator
#define BIG 1e18
// #define mod (int)(998244353)
#define mod (int)(1e9+7)
#define Tcase 0

using namespace std;

class Matrix{
public:
    const static int N=110;
    int a[N][N];
    int sz;

    Matrix(int n):sz(n){
        memset(a,0,sizeof(a));
    }

    Matrix operator*(const Matrix &b)const{
        int n=b.sz;
        Matrix res(n);
        for(int i=1;i<=n;i++){
            for(int k=1;k<=n;k++){
                if(a[i][k]==0) continue;
                for(int j=1;j<=n;j++){
                    res.a[i][j]=(res.a[i][j]+a[i][k]*b.a[k][j])%mod;
                }
            }
        }
        return res;
    }//矩阵乘法核心

    Matrix I(Matrix base){
        int n=base.sz;
        Matrix res(n);
        for(int i=1;i<=n;i++){
            res.a[i][i]=1;
        }
        return res;
    }//单位矩阵初始化

    Matrix qpow(Matrix base,int b){
        Matrix ans=I(base);
        while(b){
            if(b&1){
                ans=ans*base;
            }
            base=base*base;

            b>>=1;
        }
        return ans;
    }//矩阵快速幂
};

/*=============...solving...=============*/
void solve(){
    int n,k;
    cin>>n>>k;
    Matrix A(n);
    int x;
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++){
            cin>>x;
            A.a[i][j]=x;
        }
    }
    Matrix ANS=A.qpow(A,k);
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++) cout<<ANS.a[i][j]<<" ";
        cout<<endl;
    }
} 
/*=============...solving...=============*/

signed main(){
    ios::sync_with_stdio(0);
    cin.tie(nullptr);
    int T=1;
    if(Tcase) cin>>T;
	while(T--) solve();
    return 0;
}
/*Coding by YOLU_gargaring*/