#include<bits/stdc++.h>
#define int long long int
#define Tcase 0

using namespace std;

//模板原题: 洛谷P3402 Link: https://www.luogu.com.cn/problem/P3402
class PersistentDSU{//可持久化并查集(并查集可回退到某一版本):按大小合并,不进行路径压缩
private:
    struct Node{
        int left=0;
        int right=0;
        int value=0;
        //叶节点value<0:该点是根,-value为集合大小
        //叶节点value>0:该点的父节点编号
    };

    int n;
    vector<Node>tree;//可持久化数组节点池
    vector<int>root;//root[i]:第i次操作后的版本根

    int newNode(){
        tree.push_back(Node{});
        return (int)tree.size()-1;
    }

    int cloneNode(int pre){
        tree.push_back(tree[pre]);
        return (int)tree.size()-1;
    }

    int build(int l,int r){
        int rt=newNode();
        if(l==r){
            tree[rt].value=-1;//初始每个点自成大小为1的集合
            return rt;
        }
        int md=(l+r)>>1;
        tree[rt].left=build(l,md);
        tree[rt].right=build(md+1,r);
        return rt;
    }

    //单点修改,返回新版本的根节点,即更新属于哪个集合,可同时用作集合合并更新和集合深度更新
    int update(int pre,int l,int r,int pos,int value){
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

    int queryValue(int rt,int l,int r,int pos)const{
        if(l==r) return tree[rt].value;
        int md=(l+r)>>1;
        if(pos<=md) return queryValue(tree[rt].left,l,md,pos);
        return queryValue(tree[rt].right,md+1,r,pos);
    }

    int findRoot(int versionRoot,int x)const{//当前版本的根节点，即属于哪个集合
        int value=queryValue(versionRoot,1,n,x);
        while(value>0){
            x=value;
            value=queryValue(versionRoot,1,n,x);
        }
        return x;
    }

public://对外接口区
    //可持久化并查集构造方法,n为最初集合的个数,maxOperations为操作次数
    PersistentDSU(int n,int maxOperations):n(n){
        int height=0;
        while((1LL<<height)<n) height++;
        size_t maxNodes=2ULL*n+2ULL*maxOperations*(height+1)+5;
        tree.reserve(maxNodes);
        root.reserve(maxOperations+1);
        tree.push_back(Node{});//0号节点留空
        root.push_back(build(1,n));//版本0为初始状态
    }

    int uniteFrom(int version,int x,int y){//从历史版本合并并生成新版本
        int versionRoot=root[version];
        int fx=findRoot(versionRoot,x);
        int fy=findRoot(versionRoot,y);
        if(fx==fy){
            root.push_back(versionRoot);
            return (int)root.size()-1;
        }
        int sizeX=-queryValue(versionRoot,1,n,fx);
        int sizeY=-queryValue(versionRoot,1,n,fy);
        if(sizeX<sizeY){
            swap(fx,fy);
            swap(sizeX,sizeY);
        }
        int newRoot=update(versionRoot,1,n,fy,fx);//小集合根连向大集合根
        newRoot=update(newRoot,1,n,fx,-(sizeX+sizeY));
        root.push_back(newRoot);
        return (int)root.size()-1;
    }

    int backTo(int version){//回到历史版本并以该历史版本生成新版本
        root.push_back(root[version]);
        return (int)root.size()-1;
    }

    bool same(int version,int x,int y)const{//查询历史版本中,x和y是否属于同一集合
        return findRoot(root[version],x)==findRoot(root[version],y);
    }

    int componentSize(int version,int x)const{//查询历史版本中x所属的集合大小
        int fx=findRoot(root[version],x);
        return -queryValue(root[version],1,n,fx);
    }

    int versionCount()const{return (int)root.size();}
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n,m;
    cin>>n>>m;
    PersistentDSU dsu(n,m);
    for(int i=1;i<=m;i++){//i=0为初始版本
        int op;
        cin>>op;
        if(op==1){
            int a,b;
            cin>>a>>b;
            dsu.uniteFrom(i-1,a,b);//新合并即对应在版本i-1进行合并
        }
        else if(op==2){
            int ver;
            cin>>ver;
            dsu.backTo(ver);//回退到第ver次操作之后的状态即变为第ver版本的状态
        }
        else if(op==3){
            int a,b;
            cin>>a>>b;
            cout<<dsu.same(i-1,a,b)<<endl;//新查询即对应在版本i-1进行查询
            dsu.backTo(i-1);//查询操作不改变并查集状态,所以需要回退一次
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