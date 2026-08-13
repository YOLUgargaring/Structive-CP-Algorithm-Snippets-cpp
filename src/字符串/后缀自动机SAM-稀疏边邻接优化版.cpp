#include<bits/stdc++.h>
#define Tcase 0

using namespace std;

//模板原题 洛谷P3808 Link: https://www.luogu.com.cn/problem/P3808
//注: 此模板时间复杂度符合通过P3808的条件但仍有可能出现TLE
struct SAM{//后缀自动机稀疏边邻接转移版-可优化部分常数时空
    struct Node{//状态节点
        int len,link,head,cnt;
    };
    struct Edge{//转移边
        int to,nxt;
        char ch;
    };
    vector<Node>state;//状态节点池
    vector<Edge>edge;//转移边节点池
    int siz;//当前状态数
    int last;//上一个处理的状态
    long long differentSub;//当前字符串的本质不同子串个数

    int findEdge(int v,char c) const{
        for (int e=state[v].head;e!=-1;e= edge[e].nxt){
            if(edge[e].ch==c) return e;
        }
        return -1;
    }

    void addEdge(int v,char c,int u){
        edge.push_back({u,state[v].head,c});
        state[v].head=(int)edge.size()-1;
    }

    SAM(){}

    SAM(const string& s){//构造字符串s的SAM
        int n=s.size();
        init(n);
        for (char c:s) extend(c);
    }

    void init(int n){//构造只有初始节点的空SAM
        state.assign(2*n+1,Node{0,-1,-1,0});
        edge.clear();
        edge.reserve(3*n+5);
        siz=1;
        last=0;
        differentSub=0;
    }

    int extend(char c){//在字符串末尾新增一个字符更新SAM,并返回当前的最后一个状态
        int cur=siz++;
        state[cur].len=state[last].len+1;
        state[cur].link=0;
        state[cur].head=-1;
        state[cur].cnt=0;
        int p=last;
        while(p!=-1){
            int e=findEdge(p,c);
            if(e==-1){
                addEdge(p,c,cur);
                p=state[p].link;
            } 
            else break;
        }
        if(p==-1) state[cur].link = 0;    
        else{
            int e=findEdge(p,c);
            int q=edge[e].to;
            if(state[p].len+1==state[q].len) state[cur].link=q;
            else{
                int clone=siz++;
                state[clone].len=state[p].len+1;
                state[clone].link=state[q].link;
                state[clone].head=-1;
                state[clone].cnt=0;
                for(int ee=state[q].head;ee!=-1;ee=edge[ee].nxt) addEdge(clone,edge[ee].ch,edge[ee].to);
                while(p!=-1){
                    int e2=findEdge(p,c);
                    if(e2!=-1&&edge[e2].to==q){
                        edge[e2].to=clone;
                        p=state[p].link;
                    } 
                    else break;
                }
                state[q].link=state[cur].link=clone;
            }
        }
        last=cur;
        state[last].cnt++;
        differentSub+=state[last].len-state[state[last].link].len;
        return cur;
    }

    void buildCnt(){//统计每个状态子串出现次数
        int maxLen=0;
        for(int i=0;i<siz;i++) maxLen=max(maxLen,state[i].len);
        vector<int>bucket(maxLen+1,0);
        for(int i=0;i<siz;i++) bucket[state[i].len]++;
        for(int i=1;i<=maxLen;i++) bucket[i]+=bucket[i-1];
        vector<int>ord(siz);
        for(int i=siz-1;i>=0;i--) ord[--bucket[state[i].len]]=i;
        for(int i=siz-1;i>0;i--){
            int v=ord[i];
            if(state[v].link!=-1) state[state[v].link].cnt+=state[v].cnt;
        }
    }

    int queryOCC(const string& t) const{//查询模式串t在主串s中的出现次数
        int v=0;
        for(char c:t){
            int e=findEdge(v,c);
            if(e==-1) return 0;
            v=edge[e].to;
        }
        return state[v].cnt;
    }

    long long querySubNum() const{//查询当前本质不同子串的个数
        return differentSub;
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n;
    cin>>n;
    vector<string>word(n);
    for(auto &t:word) cin>>t;
    string t;
    cin>>t;
    SAM sam(t);
    sam.buildCnt();
    int ans=0;
    for(auto &s:word){
        if(sam.queryOCC(s)!=0) ans++;
    }
    cout<<ans;
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