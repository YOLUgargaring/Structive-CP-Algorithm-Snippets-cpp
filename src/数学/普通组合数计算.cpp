#include<bits/stdc++.h>
#define int long long int
#define TCASE 0

using namespace std;

constexpr int mod=998244353;

//模板原题: 洛谷B3717 Link: https://www.luogu.com.cn/problem/B3717
class Combina{//普通组合数求解,适用于C(N,M)中N,M<=10^6的范围和模数为通常情况的大质数(如998244353,1e9+7)的场景
public:
    int n;
    //用于计算阶乘和逆元求解组合数
    vector<int>fac;
    vector<int>invfc;

    int qpow(int a,int b){
        int res=1;
        while(b>0){
            if(b%2==1) res=(res*a)%mod;
            a=(a*a)%mod;
            b/=2;
        }
        return res;
    }

    Combina(int n):n(n+1),fac(n+1,0),invfc(n+1,0) {}//n为需要计算的组合数中下面的数n的最大值,一般设为N的上限

    void init(int n){//组合数求解预处理:预计算阶乘和逆元;调用组合数计算前需进行预处理调用init()
        fac[0]=1;
        for(int i=1;i<=n;i++) fac[i]=(fac[i-1]*i)%mod;
        invfc[n]=qpow(fac[n],mod-2);
        for(int i=n-1;i>=0;i--) invfc[i]=(invfc[i+1]*(i+1))%mod;
    }

    int Cnr(int n,int r){//计算组合数C(n,r):n为下面的,r为上面的
        if(r<0||r>n) return 0;
        return fac[n]*invfc[r]%mod*invfc[n-r]%mod;
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int t,N;
    cin>>t>>N;
    int ans=0;
    Combina com(N);
    com.init(N);
    while(t--){
        int n,m;
        cin>>n>>m;
        int res=com.Cnr(n,m);
        ans^=res;
    }
    cout<<ans;
} 
/*=============...solving...=============*/

signed main(){
    ios::sync_with_stdio(0);
    cin.tie(nullptr);
    int T=1;
    if(TCASE) cin>>T;
    // init();
	while(T--) solve_by_YOLU();
    return 0;
}
/*Coding by YOLU_gargaring*/