#include<bits/stdc++.h>
#define int long long int
#define Tcase 0

using namespace std;

/*
    模板原题: 此源代码的solve()解决的是 CF706D
    1. CF706D Link: https://codeforces.com/contest/706/problem/D
    2. CF817E Link: https://codeforces.com/contest/817/problem/E
*/
class XTrie {
    const static int H=30;
private:
    struct Node{
        Node *child[2];//存储子节点
        //每一层代表一位,只有两个子节点0/1
        int cnt;
        Node(){
            child[0]=child[1]=nullptr;
            cnt=0;
        }
    };
    vector<Node*>pool;//动态节点池,clear()后复用已经申请的节点
    int used;
    Node *root;

    Node *newNode(){
        if(used==(int)pool.size()) pool.push_back(new Node());
        Node *node=pool[used++];
        node->child[0]=node->child[1]=nullptr;
        node->cnt=0;
        return node;
    }
    
public:
    XTrie():used(0) {
        root=newNode();
    }

    XTrie(const XTrie&)=delete;
    XTrie& operator=(const XTrie&)=delete;

    ~XTrie(){
        for(Node *node:pool) delete node;
    }

    void clear(){//逻辑清空,保留已经申请的动态节点以便复用
        used=0;
        root=newNode();
    }

    void insert(int x){//插入数到异或字典树
        Node *node=root;
        node->cnt++;
        for(int i=H;i>=0;i--){
            int b=(x>>i)&1;
            if(node->child[b]==nullptr) node->child[b]=newNode();
            node=node->child[b];
            node->cnt++;
        }
    }

    void erase(int x){//删除异或字典树中的一个数字(只是cnt置0,未实际删除)
        Node *node=root;
        Node *path[H+2];
        path[0]=node;
        for(int i=H;i>=0;i--){
            int b=(x>>i)&1;
            if(node->child[b]==nullptr||node->child[b]->cnt==0) return;
            node=node->child[b];
            path[H-i+1]=node;
        }
        for(int i=0;i<=H+1;i++) path[i]->cnt--;
    }

    int queryMax(int x){//查询字典树中的数能和x异或结果的最大值
        if(root->cnt==0) return -1;
        Node *node=root;
        int res=0;
        for(int i=H;i>=0;i--){
            int b=(x>>i)&1;
            if(node->child[b^1]&&node->child[b^1]->cnt){
                res|=(1LL<<i);
                node=node->child[b^1];
            }
            else node=node->child[b];
        }
        return res;
    }

    int queryMin(int x){//查询字典树中的数能和x异或结果的最小值
        if(root->cnt==0) return -1;
        Node *node=root;
        int res=0;
        for(int i=H;i>=0;i--){
            int b=(x>>i)&1;
            if(node->child[b]&&node->child[b]->cnt){
                node=node->child[b];
            }
            else{
                res|=(1LL<<i);
                node=node->child[b^1];
            }
        }
        return res;
    }

    int queryLess(int x,int limit){//统计字典树中有多少数与x的异或结果<limit
        Node *node=root;
        int ans=0;
        for(int i=H;i>=0;i--){
            if(!node) break;
            int xb=(x>>i)&1;
            int lb=(limit>>i)&1;
            if(lb){
                if(node->child[xb]&&node->child[xb]->cnt) ans+=node->child[xb]->cnt;
                node=node->child[xb^1];
            }
            else node=node->child[xb];
        }
        return ans;
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int q;
    cin>>q;
    XTrie T;
    T.insert(0);
    while(q--){
        char op;
        int x;
        cin>>op>>x;
        if(op=='+') T.insert(x);
        else if(op=='-') T.erase(x);
        else if(op=='?') cout<<T.queryMax(x)<<endl;
        else if(op=='!'){
            int p;
            cin>>p;
            cout<<T.queryLess(x,p)<<endl;
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