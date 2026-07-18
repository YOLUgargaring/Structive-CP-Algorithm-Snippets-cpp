#include<bits/stdc++.h>
#define i64 int64_t
#define Tcase 0

using namespace std;

//模板原题: 洛谷P3383 Link: https://www.luogu.com.cn/problem/P3383
struct LinearSieve{//线性质数筛-欧拉筛
    int n;
    vector<bool>isprime;
    vector<int>prime;

    LinearSieve(int maxn){//建立[1,n]内的质数表
        n=maxn;
        isprime.assign(n+1,true);
        build();
    }

    void build(){//欧拉筛: 每个合数只会被它的最小质因数筛掉一次,复杂度O(n)
        if(n>=0) isprime[0]=false;
        if(n>=1) isprime[1]=false;
        for(int i=2;i<=n;i++) {
            if(isprime[i]) prime.push_back(i);
            for(int j=0;j<(int)prime.size();j++) {
                int p=prime[j];
                i64 product=(i64)i*p;
                if(product>n) break;
                isprime[product]=false;
                if(i%p==0) break;
            }
        }
    }

    bool isPrime(int x){//查询x是否为质数,要求0<=x<=n
        return isprime[x];
    }

    int queryKthPrime(int k){//查询第k个质数,k从1开始计数
        return prime[k-1];
    }

    int queryPrimeCount(){//返回[1,n]内质数的总数
        return prime.size();
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n,q;
    cin>>n>>q;
    LinearSieve shai(n);
    while(q--){
        int k;
        cin>>k;
        cout<<shai.queryKthPrime(k)<<endl;
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