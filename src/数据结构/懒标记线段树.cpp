#include<bits/stdc++.h>
#define int long long int

using namespace std;

//模板原题: 洛谷P3373 Link: https://www.luogu.com.cn/problem/P3373
struct SegTree{//懒标记线段树(区间加/乘，查询区间和)
    struct node{
        int data=0;
        int lazy=0;//加值懒标记
        int mlazy=1;//乘值懒标记
    };
    int n,m;//节点数/模数
    vector<node>tree;
    SegTree(int n,int m):n(n),m(m),tree((n+1)<<2){}

    void pushup(int root){//更新当前节点
        tree[root].data=(tree[2*root].data%m+tree[2*root+1].data%m)%m;
    }
    void pushdown(int root,int l,int r){//下放懒标记
        int md=l+(r-l)/2;
        int lenL=md-l+1;
        int lenR=r-md;
        if(tree[root].mlazy!=1){//有区间同乘时优先下放乘值懒标记
            tree[2*root].data=((tree[2*root].data%m)*(tree[root].mlazy%m))%m;
            tree[2*root].lazy=((tree[2*root].lazy%m)*(tree[root].mlazy%m))%m;
            tree[2*root].mlazy=((tree[2*root].mlazy%m)*(tree[root].mlazy%m))%m;
            tree[2*root+1].data=((tree[2*root+1].data%m)*(tree[root].mlazy%m))%m;
            tree[2*root+1].lazy=((tree[2*root+1].lazy%m)*(tree[root].mlazy%m))%m;
            tree[2*root+1].mlazy=((tree[2*root+1].mlazy%m)*(tree[root].mlazy%m))%m;
            tree[root].mlazy=1;
        }
        if(tree[root].lazy!=0){//下放加值懒标记至子节点的值和懒标记
            tree[2*root].data=(tree[2*root].data%m+(tree[root].lazy*lenL)%m)%m;
            tree[2*root].lazy=(tree[2*root].lazy%m+tree[root].lazy%m)%m;
            tree[2*root+1].data=(tree[2*root+1].data%m+(tree[root].lazy*lenR)%m)%m;
            tree[2*root+1].lazy=(tree[2*root+1].lazy%m+tree[root].lazy%m)%m;
            tree[root].lazy=0;//下放后自身懒标记清零
        }
    }

    void build(vector<int>&data,int root,int l,int r){//建树
        if(l==r){
            tree[root].data=data[l]%m;//递归至叶节点终止
            return;
        }
        int md=l+(r-l)/2;
        build(data,2*root,l,md);//左子树
        build(data,2*root+1,md+1,r);//右子树
        pushup(root);
    }
    void build(vector<int>&data){build(data,1,1,n);}//对外接口,无需传入根节点和区间

    void updateP(int root,int l,int r,int p,int v){//单点修改
        if(l==r){
            tree[root].data=v;
            tree[root].lazy=0;
            return;
        }
        int md=l+(r-l)/2;
        pushdown(root,l,r);
        if(p<=md) updateP(2*root,l,md,p,v);
        else updateP(2*root+1,md+1,r,p,v);
        pushup(root);
    }
    void updateP(int p,int v){updateP(1,1,n,p,v);}//对外接口

    void updateAdd(int root,int l,int r,int ul,int ur,int v){//区间元素加同值
        if(ul>r||ur<l) return;
        if(ul<=l&&ur>=r){
            tree[root].data=(tree[root].data%m+(v*(r-l+1))%m)%m;
            tree[root].lazy=(tree[root].lazy%m+v%m)%m;
            return;
        }
        int md=l+(r-l)/2;
        pushdown(root,l,r);
        updateAdd(2*root,l,md,ul,ur,v);
        updateAdd(2*root+1,md+1,r,ul,ur,v);
        pushup(root);
    }
    void updateAdd(int ul,int ur,int v){updateAdd(1,1,n,ul,ur,v);}//对外接口

    void updateMul(int root,int l,int r,int ul,int ur,int v){//区间元素乘同值
        if(ul>r||ur<l) return;
        if(ul<=l&&ur>=r){
            tree[root].data=(tree[root].data%m*v%m)%m;
            tree[root].lazy=(tree[root].lazy%m*v%m)%m;//加值懒标记也需更新
            tree[root].mlazy=(tree[root].mlazy%m*v%m)%m;
            return;
        }
        int md=l+(r-l)/2;
        pushdown(root,l,r);
        updateMul(2*root,l,md,ul,ur,v);
        updateMul(2*root+1,md+1,r,ul,ur,v);
        pushup(root);
    }
    void updateMul(int ul,int ur,int v){updateMul(1,1,n,ul,ur,v);}//对外接口

    int query(int root,int l,int r,int ql,int qr){//查询区间和
        if(ql>r||qr<l) return 0;
        if(ql<=l&&qr>=r) return tree[root].data;//当前区间包含于查询区间，直接返回
        int md=l+(r-l)/2;
        pushdown(root,l,r);
        int lS=query(2*root,l,md,ql,qr)%m;
        int rS=query(2*root+1,md+1,r,ql,qr)%m;
        return (lS+rS)%m;
    }
    int query(int ql,int qr){return query(1,1,n,ql,qr);}//对外接口
};

/*=============...solving...=============*/
void solve(){
    int n,q,t;
    cin>>n>>q>>t;
    vector<int>a(n+1);
    for(int i=1;i<=n;i++) cin>>a[i];
    SegTree seg(n,t);
    seg.build(a);
    while(q--){
        int o;
        cin>>o;
        if(o==1){
            int x,y,k;
            cin>>x>>y>>k;
            seg.updateMul(x,y,k%t);
        }
        if(o==2){
            int x,y,k;
            cin>>x>>y>>k;
            seg.updateAdd(x,y,k%t);
        }
        if(o==3){
            int x,y;
            cin>>x>>y;
            cout<<seg.query(x,y)<<endl;
        }
    }
}
/*=============...solving...=============*/

signed main(){
    ios::sync_with_stdio(0);
    cin.tie(nullptr);
    int T=1;
    //cin>>T;
    while(T--) solve();
    return 0;
}
/*Coding by YOLU_gargaring*/