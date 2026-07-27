#include<bits/stdc++.h>
#define int long long int
 
using namespace std;

//模板原题: 洛谷P8306 Link: https://www.luogu.com.cn/problem/P8306
class Trie {
struct Node{
    unordered_map<char,Node*>child;//存储子节点
    int pass;//记录过该节点的字符串数
    int end;//记录以该节点结尾的字符串数
    bool isend;//是否为叶子节点
    priority_queue<int>q;//当前子树所有字符串已选前缀的长度,大根堆
    Node():pass(0),end(0),isend(false){}
};
private:
    Node*root;

    Node* searchPrefix(string prefix){//查询字符串前缀
        Node*node=root;
        for(char c:prefix){
            if(node->child.find(c)==node->child.end()) return nullptr;
            node=node->child[c];
        }
        return node;
    }

    Node* shortenDFS(Node*node,int dep){//后序DFS,用于求解最短压缩前缀
        Node*rt=node;
        if(node->isend) rt->q.push(dep);
        for(auto &it:node->child){
            Node*nxt=shortenDFS(it.second,dep+1);
            if(rt->q.size()<nxt->q.size()) swap(rt,nxt);//启发式合并:小堆并入大堆
            while(!nxt->q.empty()){
                rt->q.push(nxt->q.top());
                nxt->q.pop();
            }
        }
        //根节点代表空串,不能被选;字符串结尾节点已经被自身占用,不能再给别的字符串使用
        if(node!=root&&!node->isend){//不是根也不是叶子节点的情况
            rt->q.pop();//删除子树中最深的已选前缀
            rt->q.push(dep);//替换为当前更短的公共前缀
        }
        return rt;
    }

public:
    Trie() {
        root=new Node();
    }
    
    void insert(string word) {//添加字符串进字典树
        Node*node=root;
        node->pass++;
        for(char c:word){
            if(node->child.find(c)==node->child.end()) node->child[c]=new Node();
            node=node->child[c];
            node->pass++;
        }
        node->end++;
        node->isend=true;
    }

    int countPrefix(string prefix){//查询字典树中以prefix为前缀的字符串数
        Node*node=root;
        for(char c:prefix){
            if(node->child.find(c)==node->child.end()) return 0;
            node=node->child[c];
        }
        return node->pass;
    }

    int count_ST_Prefix(string word){//查询字典树中有多少字符串和word有相同前缀
        Node*node=root;
        int res=0;
        for(char c:word){
            if(node->child.find(c)==node->child.end()) return res;
            node=node->child[c];
            res+=node->end;
        }
        res+=node->pass-node->end;
        return res;
    }
    
    bool search(string word) {//查询字典树中是否存在字符串word
        Node*node=searchPrefix(word);
        return node!=nullptr&&node->isend;
    }
    
    bool startsWith(string prefix) {//查询prefix是否作为字典树中某一字符串的前缀
        return searchPrefix(prefix)!=nullptr;
    }

    int querySumLCP(string word){//查询word与字典树中所有字符串的最长公共前缀长度之和
        Node*node=root;
        int res=0;
        for(char c:word){
            if(node->child.find(c)==node->child.end()) break;
            node=node->child[c];
            res+=node->pass;
        }
        return res;
    }

    int minPrefixSum(){//所有字典树中字符串压缩为不同前缀的最小长度和
        Node*rt=shortenDFS(root,0);
        int res=0;
        while(!rt->q.empty()){
            res+=rt->q.top();
            rt->q.pop();
        }
        return res;
    }
};

void solve(){
    int n,q;
    cin>>n>>q;
    Trie trie=Trie();
    vector<string>str;
    for(int i=0;i<n;i++){
        string s;
        cin>>s;
        trie.insert(s);
        str.push_back(s);
    }
    while(q--){
        string t;
        cin>>t;
        int ans=trie.countPrefix(t);
        cout<<ans<<endl;
    }
} 
 
signed main(){
    ios::sync_with_stdio(0);
    cin.tie(nullptr);
    int T;
    cin>>T;
	//   T=1;
	  while(T--) solve();
    return 0;
}
/*Coding by YOLU_gargaring*/