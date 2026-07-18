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

//模板原题: 洛谷P4777 Link: https://www.luogu.com.cn/problem/P4777
struct ExCRT{
    vector<int>module;
    vector<int>remainder;

    void addEquation(int m,int a){//同余方程组中加入一个同余方程 x mod m=a
        module.push_back(m);
        remainder.push_back((a%m+m)%m);
    }

    int exgcd(int a,int b,int& x,int& y){//扩展欧几里得: 求 ax+by=gcd(a,b)的一组整数解
        if(b==0) {
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

    //合并全部方程,存在解时返回true,并通过ans、merged_mod返回
    //最小非负解以及所有解的公共模数: 无解时返回false
    bool query(int& ans,int& merged_mod) {
        ans=remainder[0];
        merged_mod=module[0];
        for(int i=1;i<(int)remainder.size();i++) {
            int A=remainder[i];
            int M=module[i];
            // 令新解为 ans+k*merged_mod,代入新方程后求 k
            int difference=(A-ans%M+M)%M;
            int x,y;
            int g=exgcd(merged_mod,M,x,y);
            if(difference%g!=0) return false;
            int next_mod=M/g;
            x=(x%next_mod+next_mod)%next_mod;
            int k=x*(difference/g)%next_mod;
            ans+=k*merged_mod;
            merged_mod*=next_mod;
            ans=(ans%merged_mod+merged_mod)%merged_mod;
        }
        return true;
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n=read();
    ExCRT crt;
    for(int i=1;i<=n;i++){
        int a=read();
        int b=read();
        crt.addEquation(a,b);
    }
    int ans,mmod;
    bool has=crt.query(ans,mmod);
    if(has) write(ans);
    else write(-1);
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