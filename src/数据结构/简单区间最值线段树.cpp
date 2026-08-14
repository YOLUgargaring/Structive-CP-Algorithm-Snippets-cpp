#include<bits/stdc++.h>
#define int long long int
#define MX INT_MAX/2
#define MN INT_MIN/2
#define TCASE 0

using namespace std;

//模板原题: 洛谷P1886 Link: https://www.luogu.com.cn/problem/P1886
struct SegTree{//线段树,单点修改,区间最值查询
    struct node{
        int mx=MN;
        int mn=MX;
    };
    int n;//线段树维护的区间长度
    vector<node>tree;
    SegTree(int n):n(n),tree((n+1)<<2){}

    void pushup(int root){//更新当前节点
        tree[root].mx=max(tree[2*root].mx,tree[2*root+1].mx);
        tree[root].mn=min(tree[2*root].mn,tree[2*root+1].mn);
    }

    void updateP(int root,int l,int r,int p,int v){//单点修改
        if(l==r){
            tree[root].mx=max(tree[root].mx,v);
            tree[root].mn=min(tree[root].mn,v);
            return;
        }
        int md=l+(r-l)/2;
        if(p<=md) updateP(2*root,l,md,p,v);
        else updateP(2*root+1,md+1,r,p,v);
        pushup(root);
    }
    void updateP(int p,int v){updateP(1,1,n,p,v);}//对外接口

    int queryMax(int root,int l,int r,int ql,int qr){//区间最大查询
        if(ql>r||qr<l) return MN;
        if(ql<=l&&r<=qr) return tree[root].mx;
        int md=l+(r-l)/2;
        return max(queryMax(2*root,l,md,ql,qr),queryMax(2*root+1,md+1,r,ql,qr));
    }
    int queryMax(int ql,int qr){return queryMax(1,1,n,ql,qr);}//对外接口

    int queryMin(int root,int l,int r,int ql,int qr){//区间最小查询
        if(ql>r||qr<l) return MX;
        if(ql<=l&&r<=qr) return tree[root].mn;
        int md=l+(r-l)/2;
        return min(queryMin(2*root,l,md,ql,qr),queryMin(2*root+1,md+1,r,ql,qr));
    }
    int queryMin(int ql,int qr){return queryMin(1,1,n,ql,qr);}//对外接口
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n,k;
    cin>>n>>k;
    SegTree seg(n);
    for(int i=1;i<=n;i++){
        int x;
        cin>>x;
        seg.updateP(i,x);
    }
    vector<int>ansmax;
    vector<int>ansmin;
    for(int i=1;i+k-1<=n;i++) ansmax.push_back(seg.queryMax(i,i+k-1));
    for(int i=1;i+k-1<=n;i++) ansmin.push_back(seg.queryMin(i,i+k-1));
    for(int v:ansmin) cout<<v<<" ";
    cout<<endl;
    for(int v:ansmax) cout<<v<<" ";
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