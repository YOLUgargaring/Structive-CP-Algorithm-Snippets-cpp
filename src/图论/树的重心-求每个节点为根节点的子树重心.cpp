#include<bits/stdc++.h>
#define int long long int
#define Tcase 0

using namespace std;

//模板原题: CF685B Link: https://codeforces.com/contest/685/problem/B
struct TreeCentroid{//预处理有根树中以每个节点为根的子树重心
    int n;//树的节点数
    vector<vector<int>>tr;//有根树,tr[u]只存储u的子节点
    vector<int>fa;//fa[u]:节点u的父节点
    vector<int>son;//son[u]:以u为根的子树大小,包含u自身
    vector<int>ans;//ans[u]:以u为根的子树的一个重心

    TreeCentroid(int n):n(n),tr(n+1),fa(n+1),son(n+1),ans(n+1){}

    void add(int f,int u){//添加父子关系f->u,即f是u的父节点
        fa[u]=f;
        tr[f].push_back(u);
    }

    void build(int root){//预处理所有子树大小及重心;调用query前必须先调用本函数
        vector<int>order;//记录从根向下访问节点的顺序
        stack<int>stk;//显式栈,代替递归DFS,避免树退化成链时递归层数过深
        stk.push(root);
        while(!stk.empty()){
            int u=stk.top();
            stk.pop();
            order.push_back(u);
            for(int v:tr[u]) stk.push(v);
        }
        //逆序处理order,保证计算节点u时,它的所有子节点都已经计算完成
        for(int i=(int)order.size()-1;i>=0;i--){
            int u=order[i];
            son[u]=1;
            int hson=0;//u的重儿子,即子树大小最大的子节点
            for(int v:tr[u]){
                son[u]+=son[v];
                if(hson==0||son[v]>son[hson]) hson=v;
            }
            if(hson==0){//叶节点的子树只有自身,重心就是自身
                ans[u]=u;
                continue;
            }
            //u子树的重心只可能位于重儿子到u的祖先链上
            //先从重儿子子树的重心开始,再不断向父节点调整
            ans[u]=ans[hson];
            while(son[u]-son[ans[u]]>son[u]/2){
                //删除ans[u]后,若上方剩余部分超过子树大小的一半,
                //说明当前点不是u子树的重心,需要向父节点移动
                ans[u]=fa[ans[u]];
            }
        }
    }

    int query(int u){//查询以u为根的子树重心,需先调用build()
        return ans[u];
    }

    int size(int u){//查询以u为根的子树大小,需先调用build()
        return son[u];
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n,q;
    cin>>n>>q;
    TreeCentroid T(n);
    for(int i=2;i<=n;i++){
        int fa;
        cin>>fa;
        T.add(fa,i);
    }
    T.build(1);//预处理所有子树的重心,1为根节点
    while(q--){
        int u;
        cin>>u;
        cout<<T.query(u)<<endl;
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