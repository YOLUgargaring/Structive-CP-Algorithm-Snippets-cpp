#include<bits/stdc++.h>
#define int long long int
 
using namespace std;

//模板原题: 洛谷P8306 Link: https://www.luogu.com.cn/problem/P8306
class Trie {
struct Node{
    unordered_map<char,Node*>child;//存储子节点
    int pass;//记录过该节点的单词数
    int end;//记录以该节点结尾的单词数
    bool isend;//是否为叶子节点
    Node():pass(0),end(0),isend(false){}
};
private:
    Node*root;
public:
    Trie() {
        root=new Node();
    }
    
    void insert(string word) {//添加单词进字典树
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

    int countPrefix(string prefix){//查询字典树中以prefix为前缀的单词数
        Node*node=root;
        for(char c:prefix){
            if(node->child.find(c)==node->child.end()) return 0;
            node=node->child[c];
        }
        return node->pass;
    }

    int count_ST_Prefix(string word){//查询字典树中有多少单词和word有相同前缀
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
    
    bool search(string word) {//查询字典树中是否存在单词word
        Node*node=searchPrefix(word);
        return node!=nullptr&&node->isend;
    }
    
    bool startsWith(string prefix) {//查询prefix是否作为字典树中某一单词的前缀
        return searchPrefix(prefix)!=nullptr;
    }
    
    Node* searchPrefix(string prefix){//查询单词前缀
        Node*node=root;
        for(char c:prefix){
            if(node->child.find(c)==node->child.end()) return nullptr;
            node=node->child[c];
        }
        return node;
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