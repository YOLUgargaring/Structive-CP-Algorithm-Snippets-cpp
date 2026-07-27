#include<bits/stdc++.h>
#define i64 int64_t
#define Tcase 0

using namespace std;

//模板原题: 洛谷P3383 Link: https://www.luogu.com.cn/problem/P3383
class LinearSieve{//线性质数筛-欧拉筛
private:
    int n;
    vector<bool>isprime;//判断是否是质数
    vector<int>spf;//每个数的最小质因数
    vector<int>prime;//1-n内的质数

public:
    LinearSieve(int maxn):n(maxn){//建立[1,n]内的质数表
        isprime.assign(n+1,true);
        spf.assign(n+1,0);
        build();
    }

    void build(){//欧拉筛: 每个合数只会被它的最小质因数筛掉一次,复杂度O(n)
        if(n>=0) isprime[0]=false;
        if(n>=1) isprime[1]=false;
        for(int i=2;i<=n;i++) {
            if(isprime[i]){
                spf[i]=i;
                prime.push_back(i);
            }
            for(int j=0;j<(int)prime.size();j++) {
                int p=prime[j];
                i64 product=(i64)i*p;
                if(product>n) break;
                isprime[product]=false;
                spf[product]=p;
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

    int queryPrimeCount(){//查询[1,n]内质数的总数
        return prime.size();
    }

    int querySPF(int x){//查询x的最小质因数
        return spf[x];
    }

    vector<int> queryFac(int x){//质因数分解x并返回x的质因数
        vector<int>fac;
        while(x>1){
            int p=spf[x];
            fac.push_back(p);
            while(x%p==0) x/=p;
        }
        return fac;
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