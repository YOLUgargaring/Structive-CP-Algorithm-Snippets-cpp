#include<bits/stdc++.h>
#define int __int128
#define Tcase 0

using namespace std;

inline int read(){
	int x=0,f=1;
	char ch=getchar();
	while (ch<'0'||ch>'9'){if (ch=='-') f=-1;ch=getchar();}
	while (ch>='0'&&ch<='9'){x=x*10+ch-48;ch=getchar();}
	return x*f;
}//fast input number

inline void write(int x){
    if(x<0){
        putchar('-');
		x=-x;
	}
    if(x>9){
        write(x/10);
	}
    putchar(x%10+'0');
    return;
}//fast output number

//模板原题: 洛谷P4720 Link: https://www.luogu.com.cn/problem/P4720
struct ExLucasCombination{//扩展Lucas定理求解组合数对一个小整数取模
    //注: 该算法只适用于n很大模数小的场景,模数较大,如通常情况下很多题目的mod为1e9+7,998244353时,该算法不适用!
    int mod;
    ExLucasCombination(int p){
        mod=p;
    }

    int exgcd(int a,int b,int& x,int& y){//扩展欧几里得: 求ax+by=gcd(a,b)的一组整数解
        if(b==0){
            x=1;
            y=0;
            return a;
        }
        int xt,yt;
        int g=exgcd(b,a%b,xt,yt);
        x=yt;
        y=xt-a/b*yt;
        return g;
    }

    int qpow(int a,int b,int p){//快速幂计算 a^b mod p
        int res=1;
        int base=a%p;
        while(b){
            if(b&1) res=res*base%p;
            base=base*base%p;
            b>>=1;
        }
        return res;
    }

    int inv(int a,int m){//当 gcd(a,m)=1 时,用扩展欧几里得求a在模m意义下的逆元
        int x,y;
        exgcd(a,m,x,y);
        return (x%m+m)%m;
    }

    int countPrime(int n,int p){//计算 n! 中质因子p的个数
        int ret=0;
        while(n){
            ret+=n/p;
            n/=p;
        }
        return ret;
    }

    int factorialWithoutPrime(int n,int p,int pk){//计算 n! 去掉所有质因子p后,对质数幂pk取模的结果
        if(n==0) return 1;
        int block=1;
        for(int i=2;i<=pk;i++){
            if(i%p) block=block*i%pk;
        }
        int ret=qpow(block,n/pk,pk);
        for(int i=2;i<=n%pk;i++){
            if(i%p) ret=ret*i%pk;
        }
        return ret*factorialWithoutPrime(n/p,p,pk)%pk;
    }

    int combinationPrimePower(int n,int r,int p,int pk){//计算 C(n,r) mod p^k,其中 pk=p^k
        if(r>n) return 0;
        int a=factorialWithoutPrime(n,p,pk);
        int b=factorialWithoutPrime(r,p,pk);
        int c=factorialWithoutPrime(n-r,p,pk);
        int exponent=countPrime(n,p)-countPrime(r,p)-countPrime(n-r,p);
        return a*inv(b,pk)%pk*inv(c,pk)%pk*qpow(p,exponent,pk)%pk;
    }

    //用exCRT合并各个互质质数幂下的答案
    bool merge(vector<int>& remainder,vector<int>& module,int& ans,int& merged_mod){
        ans=remainder[0];
        merged_mod=module[0];
        for(int i=1;i<(int)remainder.size();i++){
            int A=remainder[i];
            int M=module[i];
            int difference=(A-ans%M+M)%M;
            int x,y;
            int g=exgcd(merged_mod,M,x,y);
            if(difference%g!=0) return false;
            int next_mod=M/g;
            x=(x%next_mod+next_mod)%next_mod;
            x=x*(difference/g)%next_mod;
            ans+=x*merged_mod;
            merged_mod*=next_mod;
            ans=(ans%merged_mod+merged_mod)%merged_mod;
        }
        return true;
    }

    //模板对外使用的主接口: 计算 C(n,r) mod mod
    int C(int n,int r) {
        if(r>n||mod==1) return 0;
        vector<int> remainder,module;
        int rest=mod;
        for(int p=2;p*p<=rest;p++) {
            if(rest%p==0) {
                int pk=1;
                while(rest%p==0){
                    pk*=p;
                    rest/=p;
                }
                remainder.push_back(combinationPrimePower(n,r,p,pk));
                module.push_back(pk);
            }
        }
        if(rest>1){
            remainder.push_back(combinationPrimePower(n,r,rest,rest));
            module.push_back(rest);
        }
        int ans,merged_mod;
        merge(remainder,module,ans,merged_mod);
        return ans%merged_mod;
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n=read();
    int m=read();
    int p=read();
    ExLucasCombination lucas(p);
    int ans=lucas.C(n,m);
    write(ans);
} 
/*=============...solving...=============*/

signed main(){
    // ios::sync_with_stdio(0);
    // cin.tie(nullptr);
    int T=1;
    if(Tcase) T=read();
	while(T--) solve_by_YOLU();
    return 0;
}
/*Coding by YOLU_gargaring*/