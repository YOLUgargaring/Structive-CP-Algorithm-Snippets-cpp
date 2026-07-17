#include<bits/stdc++.h>
#define int long long int
#define Tcase 0

using namespace std;

//模板原题: 洛谷P3834 Link: https://www.luogu.com.cn/problem/P3834
class PersistentKthTree{//主席树:用于查询静态区间第k小
private:
    struct Node{
        int left=0;//左子节点编号
        int right=0;//右子节点编号
        int count=0;//所维护的信息
    };

    int n;
    int valueCount;
    vector<int>values;//离散化后的有序原值
    vector<Node>tree;//动态节点池
    vector<int>root;//root[i]:前i个数构成的版本

    int cloneNode(int pre){
        tree.push_back(tree[pre]);
        return (int)tree.size()-1;
    }

    int update(int pre,int l,int r,int pos){//可持久化更新,返回新版本的根节点
        int rt=cloneNode(pre);
        tree[rt].count++;
        if(l==r) return rt;
        int md=(l+r)>>1;
        if(pos<=md) tree[rt].left=update(tree[pre].left,l,md,pos);
        else tree[rt].right=update(tree[pre].right,md+1,r,pos);
        return rt;
    }

    //在两个版本leftRoot,rightRoot的权值树上进行二分查询区间[l,r]的第k小
    int queryKth(int leftRoot,int rightRoot,int l,int r,int k)const{
        if(l==r) return l;
        int leftCount=tree[tree[rightRoot].left].count-tree[tree[leftRoot].left].count;
        int md=(l+r)>>1;
        if(k<=leftCount){
            return queryKth(tree[leftRoot].left,tree[rightRoot].left,l,md,k);
        }
        return queryKth(tree[leftRoot].right,tree[rightRoot].right,md+1,r,k-leftCount);
    }

public://对外接口区
    //静态区间K'th主席数构造方法,a[]为传入的原数组(1-based)
    explicit PersistentKthTree(const vector<int>&a):n((int)a.size()-1),values(a.begin()+1,a.end()),root(n+1){
        sort(values.begin(),values.end());
        values.erase(unique(values.begin(),values.end()),values.end());//离散化
        valueCount=(int)values.size();
        int height=0;
        while((1LL<<height)<valueCount) height++;
        tree.reserve(1ULL+n*(height+1));
        tree.push_back(Node{});//0号空节点同时代表空版本
        for(int i=1;i<=n;i++){
            int pos=(int)(lower_bound(values.begin(),values.end(),a[i])-values.begin())+1;
            root[i]=update(root[i-1],1,valueCount,pos);
        }
    }

    int queryKth(int l,int r,int k)const{//查询a[l..r]中的第k小,1-based
        int rank=queryKth(root[l-1],root[r],1,valueCount,k);
        return values[rank-1];//排名映射返回原值,由于原数组1-based所以需要-1
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n,m;
    cin>>n>>m;
    vector<int>a(n+1);
    for(int i=1;i<=n;i++) cin>>a[i];
    PersistentKthTree prs(a);
    while(m--){
        int l,r,k;
        cin>>l>>r>>k;
        cout<<prs.queryKth(l,r,k)<<endl;
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