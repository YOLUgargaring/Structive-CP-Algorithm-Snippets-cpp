#include<bits/stdc++.h>
#define int long long int
#define TCASE 0

using namespace std;

/*
    模板例题: 此源代码(包括封装结构体)的solve()解决的是洛谷P13979
    1.数列分块求解区间加与单点查询: 洛谷P13979 Link: https://www.luogu.com.cn/problem/P13979
    2.数列分块求解区间加与区间和查询: 洛谷P13976 Link: https://www.luogu.com.cn/problem/P13976
    注: 此类比较简单的区间加与区间和查询同样可采用线段树求解
*/
struct SRD{//数列分块-普通区间加区间求和
    int n;//原数组大小
    int B;//块大小
    int cnt;//块数
    vector<int>a;//原数组
    vector<int>belong;//belong[i]:下标i属于的块编号
    vector<int>L;//L[i]:第i块的左边界
    vector<int>R;//R[i]:第i块的有边界
    vector<int>tag;//tag[i]:第i块的加法标记
    vector<int>sum;//sum[i]:第i块的和标记

    SRD(int n):n(n){
        B=max(1LL,(int)sqrt(n));
        cnt=(n+B-1)/B;
        a.assign(n+1,0);
        belong.assign(n+1,0);
        L.assign(cnt+1,0);
        R.assign(cnt+1,0);
        tag.assign(cnt+1,0);
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

    // 整块增加,第p个块加法懒标记增加
    void addBlock(int p,int d){
        tag[p]+=d;
        sum[p]+=d*(R[p]-L[p]+1);
    }

    // 块内暴力修改
    void addPart(int l,int r,int d){
        for(int i=l;i<=r;i++) a[i]+=d;
        sum[belong[l]]+=(r-l+1)*d;
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

    // 暴力查询块内[l,r]和
    int queryPartSum(int l,int r){
        int res=0;
        for(int i=l;i<=r;i++){
            res+=a[i]+tag[belong[i]];
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
        int op,l,r,c;
        cin>>op>>l>>r>>c;
        if(op) cout<<(srd.queryRangeSum(l,r)%(c+1)+(c+1))%(c+1)<<endl;
        else srd.rangeAdd(l,r,c);
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