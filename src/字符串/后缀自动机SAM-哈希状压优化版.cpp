#include<bits/stdc++.h>
#define TCASE 0

using namespace std;

//模板原题: 洛谷P5357 Link: https://www.luogu.com.cn/problem/P5357
//注: 此模板时间复杂度符合通过P5357的条件但仍有可能出现TLE
struct SAM{//后缀自动机哈希状压优化版-时空复杂度均O(N),达到理论最佳,适用于字符集很小情况
    struct Node{//状态节点
        int len,link,head,cnt;
    };
    struct Edge{//转移边
        uint32_t to_ch;//高位存目标状态,低5位存字符编号
        int nxt;
    };
    vector<Node>state;//状态节点池
    vector<Edge>edge;//转移边节点池
    vector<uint64_t>transHash;//构造阶段的转移哈希表,统一存储(状态,字符)->边编号
    int siz;//当前状态数
    int maxLen;//当前SAM中的最大串长
    uint32_t hashMask;//手写哈希表大小减一,哈希表大小为2的幂次
    int hashShift;
    bool hashWorking;//是否仍处于SAM构造阶段
    int last;//上一个处理的状态
    long long differentSub;//当前字符串的本质不同子串个数

    uint32_t hashPosition(uint32_t key) const{
        uint32_t x=uint32_t(uint64_t(key+1)*2654435761u);
        return x>>(32-hashShift);
    }

    void setHash(int v,int ch,int e){
        uint32_t key=(uint32_t(v)<<5)|uint32_t(ch);
        uint32_t pos=hashPosition(key);
        uint64_t value=(uint64_t(key+1)<<32)|uint32_t(e+1);
        while(transHash[pos]){
            if((uint32_t)(transHash[pos]>>32)==key+1){
                transHash[pos]=value;
                return;
            }
            pos=(pos+1)&hashMask;
        }
        transHash[pos]=value;
    }

    int findEdge(int v,char c) const{
        int ch=c-'a';
        if(hashWorking){
            uint32_t key=(uint32_t(v)<<5)|uint32_t(ch);
            uint32_t pos=hashPosition(key);
            while(transHash[pos]){
                uint64_t value=transHash[pos];
                if((uint32_t)(value>>32)==key+1) return (int)(uint32_t)value-1;
                pos=(pos+1)&hashMask;
            }
            return -1;
        }
        for (int e=state[v].head;e!=-1;e=edge[e].nxt){
            if((edge[e].to_ch&31)==uint32_t(ch)) return e;
        }
        return -1;
    }

    void addEdge(int v,char c,int u){
        int ch=c-'a';
        edge.push_back({(uint32_t(u)<<5)|uint32_t(ch),state[v].head});
        int e=(int)edge.size()-1;
        state[v].head=e;
        if(hashWorking) setHash(v,ch,e);
    }

    void releaseHash(){//SAM构造完成后释放哈希表,降低后续统计阶段的空间占用
        vector<uint64_t>().swap(transHash);
        hashMask=0;
        hashWorking=false;
    }

    SAM(){}

    SAM(const string& s){//构造字符串s的SAM
        int n=s.size();
        init(n);
        for(char c:s) extend(c);
    }

    void init(int n){//构造只有初始节点的空SAM
        state.clear();
        state.reserve(2*n+1);
        state.push_back(Node{0,-1,-1,0});
        edge.clear();
        edge.reserve(3*n+5);
        int hashSize=1;
        hashShift=0;
        int maxEdge=3*n+5;
        while(hashSize<(long long)maxEdge*4/3){
            hashSize<<=1;
            hashShift++;
        }
        transHash.assign(hashSize,0);
        hashMask=hashSize-1;
        hashWorking=true;
        siz=1;
        maxLen=0;
        last=0;
        differentSub=0;
    }

    int extend(char c){//在字符串末尾新增一个字符更新SAM,并返回当前的最后一个状态
        int cur=siz++;
        state.push_back(Node{0,0,-1,0});
        state[cur].len=state[last].len+1;
        state[cur].link=0;
        state[cur].head=-1;
        state[cur].cnt=0;
        maxLen=max(maxLen,state[cur].len);
        int p=last;
        int e=-1;
        while(p!=-1&&(e=findEdge(p,c))==-1){
            addEdge(p,c,cur);
            p=state[p].link;
        }
        if(p==-1) state[cur].link = 0;    
        else{
            int q=edge[e].to_ch>>5;
            if(state[p].len+1==state[q].len) state[cur].link=q;
            else{
                int clone=siz++;
                state.push_back(Node{0,0,-1,0});
                state[clone].len=state[p].len+1;
                state[clone].link=state[q].link;
                state[clone].head=-1;
                state[clone].cnt=0;
                for(int ee=state[q].head;ee!=-1;ee=edge[ee].nxt){
                    char nextChar=(char)('a'+(edge[ee].to_ch&31));
                    int to=edge[ee].to_ch>>5;
                    addEdge(clone,nextChar,to);
                }
                while(p!=-1&&e!=-1&&int(edge[e].to_ch>>5)==q){
                    edge[e].to_ch=(uint32_t(clone)<<5)|(edge[e].to_ch&31);
                    p=state[p].link;
                    if(p!=-1) e=findEdge(p,c);
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
        releaseHash();
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
            v=edge[e].to_ch>>5;
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
    vector<string>query(n);
    for(auto &q:query) cin>>q;
    string s;
    cin>>s;
    SAM sam(s);
    sam.buildCnt();
    unordered_map<string,int>cache;
    for(auto &t:query){
        if(cache.count(t)) cout<<cache[t]<<endl;
        else{
            int ans=sam.queryOCC(t);
            cache[t]=ans;
            cout<<ans<<endl;
        }
    }
} 
/*=============...solving...=============*/

signed main(){
    ios::sync_with_stdio(0);
    cin.tie(nullptr);
    int T=1;
    if(TCASE) cin>>T;
    // init();
	while(T--) solve_by_YOLU();
    return 0;
}
/*Coding by YOLU_gargaring*/