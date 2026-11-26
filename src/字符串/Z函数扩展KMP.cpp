#include<bits/stdc++.h>
#define TCASE 0

using namespace std;

/*
    模板例题: 此源代码的solve()解决的是洛谷P5410
    1.Z-box求解与扩展KMP模板题: 洛谷P5410 Link: https://www.luogu.com.cn/problem/P5410
    2.Z-box运用求解出现在前缀后缀和非前后缀中间位置的最长子串: CF126B Link: https://codeforces.com/problemset/problem/126/B
*/
struct ExKMP_Z{//Z函数-扩展KMP算法
    string s,t;
    int n,m;//字符串s与t长度
    vector<int>z;//字符串s的Z函数数组,z[i]表示s和s[i:]的LCP(最长公共前缀)的长度,0-based
    vector<int>lcp;//lcp[i]:s与t[i:]的LCP的长度,0-based

    // 构造,传入原字符串s
    ExKMP_Z(string s,string t):s(s),t(t){
        n=s.size();
        m=t.size();
        z.assign(n,0);
        lcp.assign(m,0);
    }

    void build(){
        z[0]=n;
        int l=0,r=0;
        for(int i=1;i<n;i++){
            if(i<=r&&z[i-l]<r-i+1) z[i]=z[i-l];
            else{
                z[i]=max(0,r-i+1);
                while(i+z[i]<n&&s[z[i]]==s[i+z[i]]) z[i]++;
            }
            if(i+z[i]-1>r){
                l=i;
                r=i+z[i]-1;
            }
        }
        l=0,r=-1;
        for(int i=0;i<m;i++){
            if(i<=r&&z[i-l]<r-i+1) lcp[i]=z[i-l];
            else{
                lcp[i]=max(0,r-i+1);
                while(i+lcp[i]<m&&s[lcp[i]]==t[i+lcp[i]]) lcp[i]++;
            }
            if(i+lcp[i]-1>r){
                l=i;
                r=i+lcp[i]-1;
            }
        }
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    string a,b;
    cin>>a>>b;
    ExKMP_Z exkmp(b,a);
    exkmp.build();
    long long ansz=0,anslcp=0;
    auto z=exkmp.z;
    auto lcp=exkmp.lcp;
    // for(int v:z) cout<<v<<" ";
    // cout<<endl;
    // for(int v:lcp) cout<<v<<" ";
    // cout<<endl;
    for(int i=0;i<z.size();i++) ansz^=(1LL*(i+1)*(z[i]+1));
    for(int i=0;i<lcp.size();i++) anslcp^=(1LL*(i+1)*(lcp[i]+1));
    cout<<ansz<<endl<<anslcp;
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