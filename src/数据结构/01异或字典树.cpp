/*Coding by YOLU_gargaring*/
/* Codeforces and other OI competition version | Statu of this problem: NaN */
/* Rating of this problem: 2000 | CLIST: NaN */
#pragma GCC optimize("O3,unroll-loops")
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
#define vecout(name) for(auto&x:name) cout<<x<<" ";cout<<endl;
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

//The code from CF817E & CF706D
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
            cin>>p
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