#include<bits/stdc++.h>
#define int long long int
#define Tcase 0

using namespace std;

/*
    模板原题: 此源代码的solve()解决的是洛谷P3919
    1.历史版本数组单点修改,单点查询 洛谷P3919 Link: https://www.luogu.com.cn/problem/P3919
    2.历史版本数组单点修改,区间和查询 ABC453G Link: https://atcoder.jp/contests/abc453/tasks/abc453_g
*/
class PersistentArray{//主席数组:历史版本单点修改,总体复制,单点查询,区间和查询
private:
    struct Node{
        int left=0;//左子节点编号
        int right=0;//右子节点编号
        int value=0;//叶子节点:该位置的值；内部节点(pushup后):所维护区间的和
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
        tree[rt].value=tree[tree[rt].left].value+tree[tree[rt].right].value;//pushup:区间和=左子树和+右子树和
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
        tree[rt].value=tree[tree[rt].left].value+tree[tree[rt].right].value;//pushup:更新完子树后重新汇总区间和
        return rt;
    }

    int query(int rt,int l,int r,int pos)const{//历史单点查询
        if(l==r) return tree[rt].value;
        int md=(l+r)>>1;
        if(pos<=md) return query(tree[rt].left,l,md,pos);
        return query(tree[rt].right,md+1,r,pos);
    }

    int rangeSum(int rt,int l,int r,int ql,int qr)const{//历史区间和查询
        if(!rt||ql>r||qr<l) return 0;//空节点或完全不相交
        if(ql<=l&&r<=qr) return tree[rt].value;//完全覆盖,直接用pushup好的区间和
        int md=(l+r)>>1;
        int res=0;
        if(ql<=md) res+=rangeSum(tree[rt].left,l,md,ql,qr);
        if(qr>md) res+=rangeSum(tree[rt].right,md+1,r,ql,qr);
        return res;
    }

public://对外接口区
    //主席数组构造方法,a[]为被操作的原数组(1-based),maxOperations为所操作的次数
    PersistentArray(const vector<int>&a,int maxOperations):n((int)a.size()-1){
        int height=0;
        while((1LL<<height)<n) height++;
        size_t maxNodes=2ULL*n+1ULL*maxOperations*(height+1)+5;
        tree.reserve(maxNodes);
        root.reserve(maxOperations+1);
        tree.push_back(Node{});//0号节点留空(同时充当区间查询里的"空节点"哨兵,value恒为0)
        root.push_back(build(a,1,n));//版本0为初始数组
    }

    int updateFrom(int version,int pos,int value){//数组的历史版本单点修改并生成新版本
        root.push_back(update(root[version],1,n,pos,value));
        return (int)root.size()-1;
    }

    int copyFrom(int version){//复制历史版本并生成新版本(O(1),不新建任何树节点)
        root.push_back(root[version]);
        return (int)root.size()-1;
    }

    int query(int version,int pos)const{//查询数组的某个版本在位置pos的值
        return query(root[version],1,n,pos);
    }

    int queryRangeSum(int version,int l,int r)const{//查询数组的某个版本在区间[l,r]的和
        return rangeSum(root[version],1,n,l,r);
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