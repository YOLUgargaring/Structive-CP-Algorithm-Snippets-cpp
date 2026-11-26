#include<bits/stdc++.h>
#define int long long int
#define TCASE 0

using namespace std;

/*
    模板例题: 此源代码的solve()解决的是洛谷P3375
    1.KMP字符串单模式匹配与border数组求解模板题: 洛谷P3375 Link: https://www.luogu.com.cn/problem/P3375
    2.border数组运用求解字符串最小周期: 洛谷P4391 Link: https://www.luogu.com.cn/problem/P4391
*/
struct KMP{//KMP字符串单模式匹配
    int n,m;//主串长度/匹配串长度
    string _main;//主串
    string _pattern;//待匹配串
    vector<int>border;//_pattern的前缀函数数组,border[i]表示s[:i]的相等最长真前缀与真后缀的长度,0-based
    vector<int>match;//_pattern在_main的起始匹配位置

    // 构造,传入主串s和匹配串t
    KMP(string s,string t){
        _main=s;
        _pattern=t;
        n=s.size();
        m=t.size();
        border.assign(m,0);
        match.reserve(n);
    }

    // 构建_pattern的前缀数组
    void buildBorder(){
        int prefix=0;
        int i=1;
        while(i<m){
            if(_pattern[prefix]==_pattern[i]){
                prefix++;
                border[i]=prefix;
                i++;
            }
            else if(prefix>0) prefix=border[prefix-1];
            else border[i++]=0;
        }    
    }

    void build(){
        buildBorder();
        int i=0,j=0;
        while(i<_main.size()){
            if (_main[i]==_pattern[j]) {
                i++;
                j++;
                if(j==_pattern.size()) {
                    match.push_back(i-_pattern.size()+1);
                    j=border[j-1];
                }
            }
            else{
                if(j>0) j=border[j-1];
                else i++;
            }
        }
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    string s,t;
    cin>>s>>t;
    KMP kmp(s,t);
    kmp.build();
    for(int p:kmp.match) cout<<p<<endl;
    for(int x:kmp.border) cout<<x<<" ";
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