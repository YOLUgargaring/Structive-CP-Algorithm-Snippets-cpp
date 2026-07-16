/*Coding by YOLU_gargaring*/
/* Codeforces and other OI competition version | Statu of this problem: NaN */
/* Rating of this problem: NaN | CLIST: NaN */
// #pragma GCC optimize("O3,unroll-loops")
// #pragma GCC target("avx2")
#include<bits/stdc++.h>
#define int long long int
#define i32 int32_t
#define i64 int64_t
#define i128 __int128
#define ust unsigned short int
#define db long double
#define endl '\n'
#define pii pair<int,int>
#define tri tuple<int,int,int>
#define fr first
#define sc second
#define prque priority_queue 
#define unmap unordered_map
#define unset unordered_set
#define all(x) x.begin(),x.end()
#define rep0(i,l,r) for(int i=l;i<r;i++)
#define rep1(i,l,r) for(int i=l;i<=r;i++)
#define irep(i,l,r) for(int i=l;i>=r;i--)
#define range(type,name) for(auto&type:name)
#define vecin(type,name,n) vector<type>name(n);for(auto&x:name) cin>>x
#define vecoutline(name) for(auto&x:name) cout<<x<<endl;
#define vecoutlist(name) for(auto&x:name) cout<<x<<" ";
#define coutfix(x) fixed<<setprecision(x)
#define st_it set<int>::iterator
#define mst_it multiset<int>::iterator
#define list_it list<int>::iterator
#define itr ::iterator
#define MX INT_MAX
#define MN INT_MIN
#define E 1e-6
#define BIG 1e18
#define mod (int)(998244353)
// #define mod (int)(1e9+7)
#define Tcase 0

using namespace std;

class PersistentArray{//主席数组:历史版本单点修改与单点查询
private:
    struct Node{
        int left=0;//左子节点编号
        int right=0;//右子节点编号
        int value=0;//所维护的信息，区间和/次数/最值等
    };

    int n;
    vector<Node>tree;//动态节点池
    vector<int>root;//root[i]:第i个版本的根节点

    int newNode(){
        tree.push_back(Node{});
        return (int)tree.size()-1;
    }

    int cloneNode(int pre){
        tree.push_back(tree[pre]);
        return (int)tree.size()-1;
    }

    int build(const vector<int>&a,int l,int r){
        int rt=newNode();
        if(l==r){
            tree[rt].value=a[l];
            return rt;
        }
        int md=(l+r)>>1;
        tree[rt].left=build(a,l,md);
        tree[rt].right=build(a,md+1,r);
        return rt;
    }

    int update(int pre,int l,int r,int pos,int value){//单点修改
        int rt=cloneNode(pre);
        if(l==r){
            tree[rt].value=value;
            return rt;
        }
        int md=(l+r)>>1;
        if(pos<=md) tree[rt].left=update(tree[pre].left,l,md,pos,value);
        else tree[rt].right=update(tree[pre].right,md+1,r,pos,value);
        return rt;
    }

    int query(int rt,int l,int r,int pos)const{//历史单点查询
        if(l==r) return tree[rt].value;
        int md=(l+r)>>1;
        if(pos<=md) return query(tree[rt].left,l,md,pos);
        return query(tree[rt].right,md+1,r,pos);
    }

public://对外接口区
    //主席数组构造方法,a[]为被操作的原数组(1-based),maxOperations为所操作的次数
    PersistentArray(const vector<int>&a,int maxOperations):n((int)a.size()-1){
        int height=0;
        while((1LL<<height)<n) height++;
        size_t maxNodes=2ULL*n+1ULL*maxOperations*(height+1)+5;
        tree.reserve(maxNodes);
        root.reserve(maxOperations+1);
        tree.push_back(Node{});//0号节点留空
        root.push_back(build(a,1,n));//版本0为初始数组
    }

    int updateFrom(int version,int pos,int value){//数组的历史版本单点修改并生成新版本
        root.push_back(update(root[version],1,n,pos,value));
        return (int)root.size()-1;
    }

    int copyFrom(int version){//复制历史版本并生成新版本
        root.push_back(root[version]);
        return (int)root.size()-1;
    }

    int query(int version,int pos)const{//查询数组的某个版本在位置pos的值
        return query(root[version],1,n,pos);
    }

    int versionCount()const{return (int)root.size();}//查询版本数
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n,m;
    cin>>n>>m;
    vector<int>a(n+1);
    for(int i=1;i<=n;i++) cin>>a[i];
    PersistentArray prs(a,m);
    while(m--){
        int ver,op,pos;
        cin>>ver>>op>>pos;
        if(op==1){
            int v;
            cin>>v;
            prs.updateFrom(ver,pos,v);//此模板单点更新操作会自动生成新版本
        }
        else if(op==2){
            int ans=prs.query(ver,pos);
            prs.copyFrom(ver);//题目中说明查询后也要复制并生成新版本
            cout<<ans<<endl;
        }
    }
} 
/*=============...solving...=============*/

signed main(){
    ios::sync_with_stdio(0);
    cin.tie(nullptr);
    int T=1;
    if(Tcase) cin>>T;
	while(T--) solve_by_YOLU();
    return 0;
}
/*Coding by YOLU_gargaring*/