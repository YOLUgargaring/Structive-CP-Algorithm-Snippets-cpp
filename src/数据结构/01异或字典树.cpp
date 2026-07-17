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
    Node *root;
    
public:
    XTrie() {
        root=new Node();
    }
    void insert(int x){//插入数到异或字典树
        Node *node=root;
        for(int i=H;i>=0;i--){
            int b=(x>>i)&1;
            if(node->child[b]==nullptr) node->child[b]=new Node();
            node=node->child[b];
            node->cnt++;
        }
    }

    void erase(int x){//删除异或字典树中的一个数字(只是cnt置0,未实际删除)
        Node *node=root;
        for(int i=H;i>=0;i--){
            int b=(x>>i)&1;
            node=node->child[b];
            node->cnt--;
        }
    }

    int queryMax(int x){//查询字典树中的数能和x异或结果的最大值
        Node *node=root;
        int res=0;
        for(int i=H;i>=0;i--){
            int b=(x>>i)&1;
            if(node->child[b^1]&&node->child[b^1]->cnt){
                res|=(1<<i);
                node=node->child[b^1];
            }
            else node=node->child[b];
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