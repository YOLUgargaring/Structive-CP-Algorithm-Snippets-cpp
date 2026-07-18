#include<bits/stdc++.h>
#define int long long int
#define Tcase 1

using namespace std;

//模板原题: 洛谷P3807 Link: https://www.luogu.com.cn/problem/P3807
struct LucasCombination{//Lucas定理求解组合数模小质数
    //注: 该算法只适用于n很大模数小的场景,模数较大,如通常情况下很多题目的mod为1e9+7,998244353时,该算法不适用!
    int p;
    vector<int>fac;

    LucasCombination(int mod){//Lucas定理要求模数p为质数: 构造时预处理 0! 到 (p-1)!
        p=mod;
        fac.assign(p,1);
        for(int i=1;i<p;i++) fac[i]=fac[i-1]*i%p;
    }

    int qpow(int a,int b){//快速幂计算 a^b mod p
        int res=1;
        int base=a%p;
        while(b){
            if(b&1) res=res*base%p;
            base=base*base%p;
            b>>=1;
        }
        return res;
    }

    int inv(int x){// 费马小定理求逆元: p为质数且x不被p整除时,x^(-1)=x^(p-2) mod p
        return qpow(x,p-2);
    }

    int Cmp(int n,int r){//计算 C(n,r) mod p: 这里要求 0<=n,r<p
        if(r>n) return 0;
        return fac[n]*inv(fac[r])%p*inv(fac[n-r])%p;
    }

    int C(int n,int r){//Lucas递归公式: C(n,r)=C(n/p,r/p)*C(n%p,r%p) mod p
        if(r==0) return 1;
        return C(n/p,r/p)*Cmp(n%p,r%p)%p;
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n,m,p;
    cin>>n>>m>>p;
    LucasCombination lucas(p);
    int ans=lucas.C(n+m,n);//C(n+m,n)%p,参数中前者是下面的,后者是上面的
    cout<<ans<<endl;
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