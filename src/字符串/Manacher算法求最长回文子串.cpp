#include<bits/stdc++.h>
#define int long long int
#define TCASE 0

using namespace std;

/*
    模板例题: 此源代码的solve()解决的是洛谷P3805
    1.Manacher算法求解最长回文子串模板: 洛谷P3805 Link: https://www.luogu.com.cn/problem/P3805
    2.Manacher算法p数组运用+01字符串奇偶性求解01字符串的反对称子串个数: 洛谷P3501 Link: https://www.luogu.com.cn/problem/P3501
    3.Manacher算法运用求最长回文前后缀: CF1326D2 Link: https://codeforces.com/problemset/problem/1326/D2
*/
struct Manacher{//Manacher算法求解最长回文子串
    int n;//原字符串长度
    int m;//字符间插入#延长的字符串长度
    string str;//原字符串
    vector<char>s;//字符间插入#延长的字符串
    vector<int>p;//p[i]:以i为中心的最大扩展半径
    int mxpl;//最长回文串长度
    string mxp;//最长回文串

    Manacher(const string&t){
        n=t.size();
        str=t;
        s.push_back('#');
        for(char c:str){
            s.push_back(c);
            s.push_back('#');
        }
        m=(int)s.size();
        p.assign(m,0);
        mxpl=0;
        mxp="";
    }

    // 匹配逻辑
    bool match(char a,char b){
        return a==b;
    }

    void build(){
        int c=0;
        int r=0;
        int ct=0;
        for(int i=1;i<m-1;i++){
            if(i<=r) p[i]=min(r-i,p[2*c-i]);
            while(i-p[i]-1>=0&&i+p[i]+1<m&&match(s[i-p[i]-1],s[i+p[i]+1])) p[i]++;
            if(p[i]+i>r){
                r=p[i]+i;
                c=i;
            }
            if(p[i]>mxpl){
                mxpl=p[i];
                ct=i;
            }
        }
        int st=(ct-mxpl)/2;
        mxp=str.substr(st,mxpl);
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    string s;
    cin>>s;
    Manacher mnc(s);
    mnc.build();
    cout<<mnc.mxpl;
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
