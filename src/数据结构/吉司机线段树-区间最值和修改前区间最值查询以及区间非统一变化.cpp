/*Coding by YOLU_gargaring*/
/* Codeforces and other OI competition version | Statu of this problem: NaN */
/* Rating of this problem: Luogu紫 | CLIST: NaN */
#include<bits/stdc++.h>
// #define int long long int
#define i32 int32_t
#define i64 int64_t
#define i128 __int128
#define db long double
#define endl '\n'
#define pii pair<int,int>
#define tri tuple<int,int,int>
#define fr first
#define sc second
#define td third
#define prque priority_queue 
#define all(x) x.begin(),x.end()
#define rep(x,n,t) for(int i=x;i<n;i+=t)
#define MX LLONG_MAX
#define MN LLONG_MIN
#define E 1e-6
#define st_it set<int>::iterator
#define mst_it multiset<int>::iterator
#define list_it list<int>::iterator
#define itr ::iterator
 
using namespace std;

class SegTreeBeats{//吉司机线段树:区间加/区间取min/查询区间和/查询区间最大值/查询区间历史最大值
private:
    static constexpr i64 NINF=numeric_limits<i64>::lowest()/4;//负无穷

    struct Node{
        i64 sum=0;//区间和
        i64 mx=NINF;//区间最大值
        i64 se=NINF;//区间严格次大值
        i64 hmx=NINF;//区间历史最大值
        int mxcnt=0;//最大值个数
        i64 mlazy=0;//当前最大值的加法标记
        i64 lazy=0;//当前非最大值的加法标记
        i64 hmlazy=0;//最大值加法标记的历史最大前缀
        i64 hlazy=0;//非最大值加法标记的历史最大前缀
    };

    int n;
    vector<Node>tree;

    void pushup(int rt){
        Node &p=tree[rt];
        Node &l=tree[rt<<1];
        Node &r=tree[rt<<1|1];
        p.sum=l.sum+r.sum;
        p.hmx=max(l.hmx,r.hmx);
        if(l.mx==r.mx){
            p.mx=l.mx;
            p.mxcnt=l.mxcnt+r.mxcnt;
            p.se=max(l.se,r.se);
        }
        else if(l.mx>r.mx){
            p.mx=l.mx;
            p.mxcnt=l.mxcnt;
            p.se=max(l.se,r.mx);
        }
        else{
            p.mx=r.mx;
            p.mxcnt=r.mxcnt;
            p.se=max(l.mx,r.se);
        }
    }

    void apply(int rt,int l,int r,i64 maxAdd,i64 otherAdd,i64 histMaxAdd,i64 histOtherAdd){
        Node &p=tree[rt];
        p.sum+=maxAdd*p.mxcnt+otherAdd*(r-l+1-p.mxcnt);
        p.hmx=max(p.hmx,p.mx+histMaxAdd);
        p.hmlazy=max(p.hmlazy,p.mlazy+histMaxAdd);
        p.hlazy=max(p.hlazy,p.lazy+histOtherAdd);
        p.mx+=maxAdd;
        if(p.se!=NINF) p.se+=otherAdd;
        p.mlazy+=maxAdd;
        p.lazy+=otherAdd;
    }

    void pushdown(int rt,int l,int r){
        int md=(l+r)>>1;
        i64 childMax=max(tree[rt<<1].mx,tree[rt<<1|1].mx);
        if(tree[rt<<1].mx==childMax){
            apply(rt<<1,l,md,tree[rt].mlazy,tree[rt].lazy,tree[rt].hmlazy,tree[rt].hlazy);
        }
        else{
            apply(rt<<1,l,md,tree[rt].lazy,tree[rt].lazy,tree[rt].hlazy,tree[rt].hlazy);
        }
        if(tree[rt<<1|1].mx==childMax){
            apply(rt<<1|1,md+1,r,tree[rt].mlazy,tree[rt].lazy,tree[rt].hmlazy,tree[rt].hlazy);
        }
        else{
            apply(rt<<1|1,md+1,r,tree[rt].lazy,tree[rt].lazy,tree[rt].hlazy,tree[rt].hlazy);
        }
        tree[rt].mlazy=tree[rt].lazy=0;
        tree[rt].hmlazy=tree[rt].hlazy=0;
    }

    void build(const vector<i64>&a,int rt,int l,int r){
        if(l==r){
            tree[rt].sum=tree[rt].mx=tree[rt].hmx=a[l];
            tree[rt].se=NINF;
            tree[rt].mxcnt=1;
            return;
        }
        int md=(l+r)>>1;
        build(a,rt<<1,l,md);
        build(a,rt<<1|1,md+1,r);
        pushup(rt);
    }

    //Operations:
    void rangeAdd(int rt,int l,int r,int ql,int qr,i64 value){//区间加
        if(ql<=l&&r<=qr){
            apply(rt,l,r,value,value,value,value);
            return;
        }
        pushdown(rt,l,r);
        int md=(l+r)>>1;
        if(ql<=md) rangeAdd(rt<<1,l,md,ql,qr,value);
        if(qr>md) rangeAdd(rt<<1|1,md+1,r,ql,qr,value);
        pushup(rt);
    }

    void rangeChmin(int rt,int l,int r,int ql,int qr,i64 value){//区间取min
        if(value>=tree[rt].mx) return;
        if(ql<=l&&r<=qr&&value>tree[rt].se){
            i64 delta=value-tree[rt].mx;
            apply(rt,l,r,delta,0,delta,0);
            return;
        }
        pushdown(rt,l,r);
        int md=(l+r)>>1;
        if(ql<=md) rangeChmin(rt<<1,l,md,ql,qr,value);
        if(qr>md) rangeChmin(rt<<1|1,md+1,r,ql,qr,value);
        pushup(rt);
    }

    i64 querySum(int rt,int l,int r,int ql,int qr){//查询区间和
        if(ql<=l&&r<=qr) return tree[rt].sum;
        pushdown(rt,l,r);
        int md=(l+r)>>1;
        i64 res=0;
        if(ql<=md) res+=querySum(rt<<1,l,md,ql,qr);
        if(qr>md) res+=querySum(rt<<1|1,md+1,r,ql,qr);
        return res;
    }

    i64 queryMax(int rt,int l,int r,int ql,int qr){//查询区间最值
        if(ql<=l&&r<=qr) return tree[rt].mx;
        pushdown(rt,l,r);
        int md=(l+r)>>1;
        i64 res=NINF;
        if(ql<=md) res=max(res,queryMax(rt<<1,l,md,ql,qr));
        if(qr>md) res=max(res,queryMax(rt<<1|1,md+1,r,ql,qr));
        return res;
    }

    i64 queryHistoryMax(int rt,int l,int r,int ql,int qr){//查询区间历史最值
        if(ql<=l&&r<=qr) return tree[rt].hmx;
        pushdown(rt,l,r);
        int md=(l+r)>>1;
        i64 res=NINF;
        if(ql<=md) res=max(res,queryHistoryMax(rt<<1,l,md,ql,qr));
        if(qr>md) res=max(res,queryHistoryMax(rt<<1|1,md+1,r,ql,qr));
        return res;
    }

public://对外接口区
    explicit SegTreeBeats(const vector<i64>&a):n((int)a.size()-1),tree((n<<2)+5){
        build(a,1,1,n);
    }

    void rangeAdd(int l,int r,i64 value){rangeAdd(1,1,n,l,r,value);}
    void rangeChmin(int l,int r,i64 value){rangeChmin(1,1,n,l,r,value);}
    i64 querySum(int l,int r){return querySum(1,1,n,l,r);}
    i64 queryMax(int l,int r){return queryMax(1,1,n,l,r);}
    i64 queryHistoryMax(int l,int r){return queryHistoryMax(1,1,n,l,r);}
};

/*=============...solving...=============*/
void solve(){
    int n,m;
    cin>>n>>m;
    vector<i64>a(n+1);
    for(int i=1;i<=n;i++) cin>>a[i];
    SegTreeBeats seg(a);
    while(m--){
        int op,l,r;
        cin>>op>>l>>r;
        if(op==1){
            i64 v;
            cin>>v;
            seg.rangeAdd(l,r,v);
        }
        else if(op==2){
            i64 v;
            cin>>v;
            seg.rangeChmin(l,r,v);
        }
        else if(op==3) cout<<seg.querySum(l,r)<<endl;
        else if(op==4) cout<<seg.queryMax(l,r)<<endl;
        else cout<<seg.queryHistoryMax(l,r)<<endl;
    }
}
/*=============...solving...=============*/

signed main(){
    ios::sync_with_stdio(0);
    cin.tie(nullptr);
    int T;
    //cin>>T;
	  T=1;
	  while(T--) solve();
    return 0;
}
/*Coding by YOLU_gargaring*/
