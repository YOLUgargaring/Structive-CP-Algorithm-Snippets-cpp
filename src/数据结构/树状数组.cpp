#include<bits/stdc++.h>
#define int long long int
 
using namespace std;

/*
    模板原题: 此源代码的solve()解决的是 洛谷P3374
    1.单点更新与区间和查询: 洛谷P3374 Link: https://www.luogu.com.cn/problem/P3374
    2.逆序对数查询模板: 洛谷P1908 Link: https://www.luogu.com.cn/problem/P1908    
*/
class FwTree{
private:
    int n;
    vector<int>tree;
    static int lowbit(int x){return x&(-x);}

public:
    FwTree(int size){//根据数组长度建立一棵值全为0的树状数组
        n=size;
        tree.assign(n+1,0);
    }

    FwTree(const vector<int>&a){//根据1-based原数组直接建立树状数组,不需要在外部逐点update
        build(a);
    }

    void build(const vector<int>&a){//使用1-based原数组重新建树,利用父子区间关系在线性O(n)内完成
        n=a.size()-1;
        tree.assign(n+1,0);
        for(int i=1;i<=n;i++){
            tree[i]+=a[i];
            int parent=i+lowbit(i);
            if(parent<=n) tree[parent]+=tree[i];
        }
    }

    void update(int p,int d){//单点更新
        for(int i=p;i<=n;i+=lowbit(i)) tree[i]+=d;
    }

    int query(int p){//前缀和查询,1-based
        int s=0;
        for(int i=p;i>0;i-=lowbit(i)) s+=tree[i];
        return s;
    }

    int query(int l,int r){//区间和查询,1-based
        if(l>r) return 0;
        return query(r)-query(l-1);
    }

    //对外接口:求解1-based原数组a[]的逆序对数
    //该接口属于 FwTree 类本身,可直接使用 FwTree::queryInv(a) 调用该接口进行数组a[]的逆序对数查询
    static int queryInv(const vector<int>&a){//使用方式: `FwTree::queryInv(a)` 直接返回a[]的逆序对数
        vector<int>v(a.begin()+1,a.end());
        sort(all(v));
        v.erase(unique(all(v)),v.end());//离散化
        FwTree T(v.size());
        int m=a.size()-1;
        int res=0;
        for(int i=1;i<=m;i++){
            int id=lower_bound(all(v),a[i])-v.begin()+1;
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
    FwTree T(a);
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