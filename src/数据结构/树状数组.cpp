#include<bits/stdc++.h>
#define int long long int
 
using namespace std;

/*
    模板原题: 此源代码的solve()解决的是 洛谷P3374
    1.单点更新与区间和查询: 洛谷P3374 Link: https://www.luogu.com.cn/problem/P3374
    2.逆序对数查询模板: 洛谷P1908 Link: https://www.luogu.com.cn/problem/P1908    
    3.单点异或更新与区间异或查询: ABC185-F Link: https://atcoder.jp/contests/abc185/tasks/abc185_f
*/
class FwTree{
private:
    int n;
    vector<int>tree;
    int model;//取1:区间和; 取2:区间异或; 取3:区间max
    static int lowbit(int x){return x&(-x);}

public:
    FwTree(int size,int md){//根据数组长度建立一棵值全为0的树状数组
        n=size;
        model=md;
        tree.assign(n+1,0);
        if(md<1||md>3){
            cerr<<"Wrong Fenwick Tree Model Seted!"<<endl;
            cerr<<"set 1 to range sum"<<endl;
            cerr<<"set 2 to range xor"<<endl;
            cerr<<"set 3 to prefix max"<<endl;
            exit(0);
        }
    }

    FwTree(const vector<int>&a,int md){//根据1-based原数组直接建立树状数组,不需要在外部逐点update
        model=md;
        build(a);
    }

    void build(const vector<int>&a){//使用1-based原数组重新建树,利用父子区间关系在线性O(n)内完成
        n=a.size()-1;
        tree.assign(n+1,0);
        for(int i=1;i<=n;i++){
            int parent=i+lowbit(i);
            if(model==1){
                tree[i]+=a[i];
                if(parent<=n) tree[parent]+=tree[i];
            }
            else if(model==2){
                tree[i]^=a[i];
                if(parent<=n) tree[parent]^=tree[i];
            }
            else if(model==3){
                tree[i]=max(tree[i],a[i]);
                if(parent<=n) tree[parent]=max(tree[parent],tree[i]);
            }
            else{
                cerr<<"Wrong Fenwick Tree Model Seted!"<<endl;
                cerr<<"set 1 to range sum"<<endl;
                cerr<<"set 2 to range xor"<<endl;
                cerr<<"set 3 to prefix max"<<endl;
                exit(0);
            }
        }
    }

    void update(int p,int d){//单点更新
        for(int i=p;i<=n;i+=lowbit(i)){
            if(model==1) tree[i]+=d;
            else if(model==2) tree[i]^=d;
            else if(model==3) tree[i]=max(tree[i],d);
        }
    }

    int query(int p){//前缀查询,1-based
        int s=0;
        for(int i=p;i>0;i-=lowbit(i)){
            if(model==1) s+=tree[i];
            else if(model==2) s^=tree[i];
            else if(model==3) s=max(tree[i],s);
        }
        return s;
    }

    int query(int l,int r){//区间查询,1-based
        if(l>r) return 0;
        int res;
        if(model==3){
            cerr<<"Error, this Fenwick Tree Template only allow Prefix Max Query!"<<endl;
            exit(0);
        }
        else if(model==1) res=query(r)-query(l-1);
        else if(model==2) res=query(r)^query(l-1);
        return res;
    }

    //对外接口:求解1-based原数组a[]的逆序对数
    //该接口属于 FwTree 类本身,可直接使用 FwTree::queryInv(a) 调用进行逆序对数查询
    static int queryInv(const vector<int>&a){
        vector<int>v(a.begin()+1,a.end());
        ranges::sort(v);
        v.erase(unique(v.begin(),v.end()),v.end());//离散化
        FwTree T(v.size(),1);
        int m=a.size()-1;
        int res=0;
        for(int i=1;i<=m;i++){
            int id=lower_bound(v.begin(),v.end(),a[i])-v.begin()+1;
            res+=i-1-T.query(id);//已插入i-1个数,减去小于等于a[i]的个数,剩下的均能组成逆序对
            T.update(id,1);
        }
        return res;
    }
};

/*=============...solving...=============*/
void solve(){
    int n,m;
    cin>>n>>m;
    vector<int>a(n+1);
    for(int i=1;i<=n;i++) cin>>a[i];
    FwTree T(a,1);
    while(m--){
        int op;
        cin>>op;
        if(op==1){
            int p,x;
            cin>>p>>x;
            T.update(p,x);
        }
        else{
            int l,r;
            cin>>l>>r;
            cout<<T.query(l,r)<<endl;
        }
    }
} 
/*=============...solving...=============*/

signed main(){
    ios::sync_with_stdio(0);
    cin.tie(nullptr);
    int T=1;
    // cin>>T;
	while(T--) solve();
    return 0;
}
/*Coding by YOLU_gargaring*/