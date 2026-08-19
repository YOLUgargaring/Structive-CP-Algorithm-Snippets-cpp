#include<bits/stdc++.h>
#define int long long int
#define TCASE 0

using namespace std;

/*
    模板例题: 此源代码的solve()解决的是洛谷P3809
    1.后缀排序模板: 洛谷P3809 Link: https://www.luogu.com.cn/problem/P3809
    2.求按字典序排序的循环同构: 洛谷P4051 Link: https://www.luogu.com.cn/problem/P4051
    3.求字符串的本质不同子串个数: SPOJ-705 Link: https://vjudge.net/problem/SPOJ-SUBST1
*/
/* - ↓↓↓ - The Suffix Array Template - ↓↓↓ - */
template<typename T> struct SuffixArray{//后缀数组
    int n;//字符串长度
    vector<T>S;//构建后缀数组的对象字符串S,1-based
    vector<int>sa;//后缀数组: sa[i]为将所有后缀排序后第i小的后缀的编号
    vector<int>rank;//后缀排名: rank[i]为第i个后缀的排名
    vector<int>rkcopy;//后缀排序辅助数组
    vector<int>height;//height[i]=LCP(sa[i],sa[i-1]): 第i名后缀与第i-1名后缀的最长公共前缀的长度(height[1]=0)
    const T apart=0;//不计入后缀数组和后缀排序的分隔符(sa[i]=rank[i]=0不影响字典序和排序)

    SuffixArray(){}

    static vector<char> stringToVecChar(string s){
        vector<char>vec(s.begin(),s.end());
        return vec;
    }

    SuffixArray(const vector<T>& s){//构建字符串s的后缀数组
        S.push_back(0);
        for(auto v:s) S.push_back(v);
        n=s.size();
        sa.assign(n+1,0);
        height.assign(n+1,0);
        rank.assign((n+1)<<1,0);
        rkcopy.assign((n+1)<<1,0);
        for(int i=1;i<=n;i++){
            if(S[i]==apart) continue;
            sa[i]=i;
            rank[i]=S[i];
        }
        build();
    }
    template<typename U=T,typename enable_if<is_same<U,char>::value,int>::type=0>
    SuffixArray(const string&s):SuffixArray(stringToVecChar(s)){}

    void build(){//后缀数组和后缀排序构建
        for(int i=1;i<n;i<<=1){
            sort(sa.begin()+1,sa.end(),[&](int x,int y){
                return rank[x]==rank[y]?rank[x+i]<rank[y+i]:rank[x]<rank[y];
            });
            rkcopy=rank;
            int p=0;
            for(int j=1;j<=n;j++){
                if(rkcopy[sa[j]]==rkcopy[sa[j-1]]&&rkcopy[sa[j]+i]==rkcopy[sa[j-1]+i]) rank[sa[j]]=p;
                else rank[sa[j]]=++p;
            }
        }
        int k=0;
        for(int i=1;i<=n;i++){
            if(rank[i]==0) continue;
            if(k) k--;
            while(S[i+k]==S[sa[rank[i]-1]+k]) k++;
            height[rank[i]]=k;
        }
    }

    int querySubNum(){//查询字符串S的本质不同子串个数
        int res=n*(n+1)/2;
        for(int i=2;i<=n;i++) res-=height[i];
        return res;
    }
};
using SAchar=SuffixArray<char>;
using SAint=SuffixArray<int>;
/* - ↑↑↑ - The Suffix Array Template - ↑↑↑ - */

/*=============...solving...=============*/
void solve_by_YOLU(){
    string s;
    cin>>s;
    SAchar SA(s);
    for(int i=1;i<=SA.n;i++) cout<<SA.sa[i]<<" ";
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