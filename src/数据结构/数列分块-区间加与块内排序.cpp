#include<bits/stdc++.h>
#define int long long int
#define TCASE 0

using namespace std;

/*
    模板例题: 此源代码(包括封装结构体)的solve()解决的是洛谷P13978
    1.数列分块+块内排序+二分求解区间加于查询区间内比c小的最大值: 
        洛谷P13978 Link: https://www.luogu.com.cn/problem/P13978
    2.数列分块+块内排序+二分求解区间加于查询区间内小于x的数的个数: 
        洛谷P13977 Link: https://www.luogu.com.cn/problem/P13977
    3.数列分块+块内排序+二分求解区间加于查询区间内大于等于x的数的个数: 
        洛谷P2801 Link: https://www.luogu.com.cn/problem/P2801
    注: 三道例题的分块构造,块内排序和rebuild重排的逻辑完全相同
        类似题目只需根据题意修改查询函数 -> 查询[l,r]拆分为左部不饱满块,中间完整块,右部不饱满查询
*/
struct SRD{//数列分块-区间加与块内排序处理
    const static long long LMN=LLONG_MIN/2;
    int n;//原数组大小
    int B;//块大小
    int cnt;//块数
    vector<int>a;//原数组
    vector<int>belong;//belong[i]:下标i属于的块编号
    vector<int>L;//L[i]:第i块的左边界
    vector<int>R;//R[i]:第i块的有边界
    vector<int>tag;//tag[i]:第i块的加法标记
    vector<vector<int>>sorted;//块内排序后的副本

    SRD(int n):n(n){
        B=max(1LL,(int)sqrt(n));
        cnt=(n+B-1)/B;
        a.assign(n+1,0);
        belong.assign(n+1,0);
        L.assign(cnt+1,0);
        R.assign(cnt+1,0);
        tag.assign(cnt+1,0);
        sorted.assign(cnt+1,{});
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
            for(int j=L[i];j<=R[i];j++) sorted[i].push_back(a[j]);
            ranges::sort(sorted[i]);
        }
    }

    // 第b块重新排序
    void rebuild(int b){
        sorted[b].clear();
        for(int i=L[b];i<=R[b];i++) sorted[b].push_back(a[i]);
        ranges::sort(sorted[b]);
    }

    // 整块增加,第p个块加法懒标记增加
    void addBlock(int p,int d){
        tag[p]+=d;
    }

    // 块内暴力修改
    void addPart(int l,int r,int d){
        for(int i=l;i<=r;i++) a[i]+=d;
        rebuild(belong[l]);
    }

    // 区间加
    void rangeAdd(int l,int r,int d){
        int bl=belong[l];
        int br=belong[r];
        if(bl==br){
            addPart(l,r,d);
            return;
        }
        addPart(l,R[bl],d);
        for(int i=bl+1;i<br;i++) addBlock(i,d);
        addPart(L[br],r,d);
    }

    // 单点查询
    int queryP(int p){
        int v=a[p]+tag[belong[p]];
        return v;
    }

    // 暴力查询块内[l,r]小于x的最大值
    int queryPartPreX(int l,int r,int x){
        int res=LMN;
        for(int i=l;i<=r;i++){
            if(a[i]+tag[belong[i]]<x) res=max(res,a[i]+tag[belong[i]]);
        }
        return res;
    }

    // 查询第b块中小于x的最大值
    int queryBlockPreX(int b,int x){
        int tar=x-tag[b];
        auto it=ranges::lower_bound(sorted[b],tar);
        if(it==sorted[b].begin()) return LMN;
        return *prev(it)+tag[b];
    }

    // 查询区间[l,r]小于x的最大值
    int queryRangePreX(int l,int r,int x){
        int bl=belong[l];
        int br=belong[r];
        if(bl==br){
            int res=queryPartPreX(l,r,x);
            if(res==LMN) return -1;
            return res;
        }
        int res=LMN;
        res=max(res,queryPartPreX(l,R[bl],x));
        for(int i=bl+1;i<br;i++) res=max(res,queryBlockPreX(i,x));
        res=max(res,queryPartPreX(L[br],r,x));
        if(res==LMN) return -1;
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
        int op,l,r,c;
        cin>>op>>l>>r>>c;
        if(op==0) srd.rangeAdd(l,r,c);
        else if(op==1) cout<<srd.queryRangePreX(l,r,c)<<endl;
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