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

//模板原题: 洛谷P1495 Link: https://www.luogu.com.cn/problem/P1495
struct CRT{
    //注: 此模板只适用同余方程组的模数互质的情况,不互质的情况需要扩展CRT
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

    int query(){//求同余方程组的最小非负整数解,要求所有模数两两互质
        int all_module=1;
        for(int i=0;i<(int)module.size();i++) all_module*=module[i];
        //设 Mi=all_module/module[i],再求 Mi 在模 module[i] 下的逆元
        //最终答案为 ∑(remainder[i]*Mi*inv(Mi)) mod all_module
        int ans=0;
        for(int i=0;i<(int)module.size();i++){
            int Mi=all_module/module[i];
            int x,y;
            exgcd(Mi,module[i],x,y);
            int inverse=(x%module[i]+module[i])%module[i];
            ans+=remainder[i]*Mi*inverse;
        }
        return (ans%all_module+all_module)%all_module;
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n=read();
    CRT crt;
    for(int i=1;i<=n;i++){
        int a=read();
        int b=read();
        crt.addEquation(a,b);//加入同余方程
    }
    int ans=crt.query();
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