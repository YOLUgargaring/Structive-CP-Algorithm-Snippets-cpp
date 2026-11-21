#include<bits/stdc++.h>
#define int long long int
#define TCASE 0

using namespace std;

/*
    模板例题: 
    1.数列分块求解区间开方与区间和查询: 洛谷P13980 Link: https://www.luogu.com.cn/problem/P13980
    2.与1同题意双倍经验,注意l>r的情况的交换l,r: 洛谷P4145 Link: https://www.luogu.com.cn/problem/P4145
    注: 此类经过可接受次数的区间操作后区间内的值能稳定不变的问题可套用stable[]记录稳定和rebuild重构的思路
        例如含有区间内的数/2的操作也可运用此类将区间稳定后O(1)处理块的思想
*/
struct SRD{//数列分块-区间渐进稳定处理与区间和查询
    int n;//原数组大小
    int B;//块大小
    int cnt;//块数
    vector<int>a;//原数组
    vector<int>belong;//belong[i]:下标i属于的块编号
    vector<int>L;//L[i]:第i块的左边界
    vector<int>R;//R[i]:第i块的有边界
    vector<bool>stable;//stable[i]:第i块内的数是否已稳定,≤1
    vector<int>sum;//sum[i]:第i块的和标记

    SRD(int n):n(n){
        B=max(1LL,(int)sqrt(n));
        cnt=(n+B-1)/B;
        a.assign(n+1,0);
        belong.assign(n+1,0);
        L.assign(cnt+1,0);
        R.assign(cnt+1,0);
        stable.assign(cnt+1,0);
        sum.assign(cnt+1,0);
    }

    // 传入1-based数组构建
    void build(const vector<int>&v){
        for(int i=1;i<=n;i++){
            a[i]=v[i];
            belong[i]=(i-1)/B+1;
        }
        for(int i=1;i<=cnt;i++){
            L[i]=(i-1)*B+1;
            R[i]=min(n,i*B);
        }
        for(int i=1;i<=cnt;i++){
            for(int j=L[i];j<=R[i];j++) sum[i]+=a[j];
        }
    }

    // 重新计算第b块的和并更新第b块是否已稳定
    void rebuild(int b){
        sum[b]=0;
        stable[b]=true;
        for(int i=L[b];i<=R[b];i++){
            sum[b]+=a[i];
            if(a[i]>1) stable[b]=false;
        }
    }

    // 第b块整块开方
    void sqrtBlock(int b){
        if(stable[b]) return;
        for(int i=L[b];i<=R[b];i++) a[i]=(int)sqrt(a[i]);
        rebuild(b);
    }

    // 块内[l,r]开方
    void sqrtPart(int l,int r){
        int b=belong[l];
        if(stable[b]) return;
        for(int i=l;i<=r;i++) a[i]=(int)sqrt(a[i]);
        rebuild(b);
    }

    // 区间开方
    void rangeSqrt(int l,int r){
        int bl=belong[l];
        int br=belong[r];
        if(bl==br){
            sqrtPart(l,r);
            return;
        }
        sqrtPart(l,R[bl]);
        for(int i=bl+1;i<br;i++) sqrtBlock(i);
        sqrtPart(L[br],r);
    }

    // 单点查询
    int queryP(int p){
        int v=a[p];
        return v;
    }

    // 暴力查询块内[l,r]和
    int queryPartSum(int l,int r){
        int res=0;
        for(int i=l;i<=r;i++){
            res+=a[i];
        }
        return res;
    }

    // 查询第b块的和
    int queryBlockSum(int b){
        return sum[b];
    }

    // 查询区间[l,r]的和
    int queryRangeSum(int l,int r){
        int bl=belong[l];
        int br=belong[r];
        if(bl==br){
            return queryPartSum(l,r);
        }
        int res=0;
        res+=queryPartSum(l,R[bl]);
        for(int i=bl+1;i<br;i++) res+=queryBlockSum(i);
        res+=queryPartSum(L[br],r);
        return res;
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n;
    cin>>n;
    vector<int>a(n+1);
    for(int i=1;i<=n;i++) cin>>a[i];
    SRD srd(n);
    srd.build(a);
    for(int i=1;i<=n;i++){
        int op,l,r;
        cin>>op>>l>>r;
        if(op) cout<<srd.queryRangeSum(l,r)<<endl;
        else srd.rangeSqrt(l,r);
    }
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