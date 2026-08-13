#include<bits/stdc++.h>
#define int long long int
#define TCASE 0

using namespace std;

/*
    模板原题: 此源代码的solve()解决的是洛谷P3804
    1.子串的出现次数统计: 洛谷P3804 Link: https://www.luogu.com.cn/problem/P3804
    2.本质不同子串个数在线查询: 洛谷P4070 Link: https://www.luogu.com.cn/problem/P4070
    3.求解两个字符串的最长公共子串LCS: SPOJ1811 Link: https://vjudge.net/problem/SPOJ-LCS#author=main
    4.求解多个字符串的共同最长公共子串LCS: SPOJ1812 Link: https://vjudge.net/problem/SPOJ-LCS2#author=main
    5.查询字典序第K小子串-分位置不同和本质不同两种情况: 洛谷P3975 Link: https://www.luogu.com.cn/problem/P3975
    6.多次查询字典序第K小子串-本质不同情况: SPOJ7258 Link: https://vjudge.net/problem/SPOJ-SUBLEX#author=main
*/
struct SAM{//后缀自动机
    struct node{//状态节点
        int len;//该状态对应的最长字串长度
        int link;//该状态的后缀链接
        map<char,int>nxt;//转移列表
    };
    vector<node>state;//状态节点池
    vector<int>cnt;//当前状态里所有子串的出现次数(一个状态的子串出现次数都相同)
    vector<int>ord;//状态序列,通常按len[v]降序排列
    vector<int>dp;//查询本质不同第K小子串所用DP
    vector<int>dpL;//查询位置不同第K小子串所用DP
    int maxn;//最大状态数
    int siz;//当前状态数
    int last;//上一个处理的状态
    int differentSub;//当前字符串的本质不同子串个数
    bool ordBuilt;
    bool cntBuilt;
    bool dpBuilt;

    SAM(){}

    SAM(string s){//构造字符串s的SAM
        int n=s.size();
        initEmpty(n);
        for(const auto&c:s) extend(c);
    }

    void initEmpty(int n){//构造只有初始节点的空SAM
        maxn=2*n+1;
        state.assign(maxn,{});
        cnt.assign(maxn,0);
        state[0].len=0;
        state[0].link=-1;
        siz=1;
        last=0;
        differentSub=0;
        ordBuilt=false;
        cntBuilt=false;
        dpBuilt=false;
    }

    int extend(char c){//在字符串末尾新增一个字符更新SAM,并返回当前的最后一个状态
        int cur=siz++;
        state[cur].len=state[last].len+1;
        int p=last;
        while(p!=-1&&!state[p].nxt.count(c)){
            state[p].nxt[c]=cur;
            p=state[p].link;
        }
        if(p==-1) state[cur].link=0;
        else{
            int q=state[p].nxt[c];
            if(state[p].len+1==state[q].len) state[cur].link=q;
            else{
                int clone=siz++;
                state[clone].len=state[p].len+1;
                state[clone].nxt=state[q].nxt;
                state[clone].link=state[q].link;
                while(p!=-1&&state[p].nxt[c]==q){
                    state[p].nxt[c]=clone;
                    p=state[p].link;
                }
                state[q].link=state[cur].link=clone;
            }
        }
        last=cur;
        cnt[last]++;
        //SAM中的一个状态v对本质不同子串个数的贡献为len[v]-len[link[v]]
        differentSub+=state[last].len-state[state[last].link].len;
        return cur;
    }

    void buildOrd(){//构造按len降序排列的状态序列
        if(ordBuilt) return;
        ord.assign(siz,0);
        iota(ord.begin(),ord.end(),0);
        sort(ord.begin(),ord.end(),[&](int x,int y){return state[x].len<state[y].len;});
        reverse(ord.begin(),ord.end());
        ordBuilt=true;
    }

    //字符串末尾新增字符后状态更新,再次调用buildOrd,buildCnt,buildDP需要重置ord,cnt,dp
    void ordReset(){
        ordBuilt=false;
        cntBuilt=false;
        dpBuilt=false;
    }

    void buildCnt(){//统计每个状态子串出现次数
        if(cntBuilt) return;
        buildOrd();
        for(int v:ord){
            if(state[v].link!=-1) cnt[state[v].link]+=cnt[v];
        }
        cntBuilt=true;
    }

    void buildDP(){
        if(dpBuilt) return;
        buildOrd();
        dp.assign(siz+1,0);
        dpL.assign(siz+1,0);
        buildCnt();
        for(int v:ord){
            dp[v]=(v==0)?0:1;
            dpL[v]=(v==0)?0:cnt[v];
            for(const auto&[c,u]:state[v].nxt) dp[v]+=dp[u];
            for(const auto&[c,u]:state[v].nxt) dpL[v]+=dpL[u];
        }
        dpBuilt=true;
    }

    int querySubNum(){//查询当前本质不同子串的个数
        return differentSub;
    }

    int queryOCC(string t){//查询模式串t在主串s中的出现次数
        int v=0;
        for(char c:t){
            if(!state[v].nxt.count(c))
                return 0;
            v=state[v].nxt[c];
        }
        return cnt[v];
    }

    //查询字符串的第K小子串: type=0 查询本质不同第K小;type=1 查询位置不同第K小
    string queryKthSub(int k,int type){
        string res="";
        buildDP();
        if(type==0){
            if(dp[0]<k) return "-1";
            int v=0;
            while(1){
                int wv=(v==0)?0:1;
                if(k<=wv) return res;
                k-=wv;
                bool found=false;
                for(const auto&[c,u]:state[v].nxt){
                    if(k>dp[u]) k-=dp[u];
                    else{
                        res+=c;
                        v=u;
                        found=true;
                        break;
                    }
                }
                if(!found) return "-1";
            }
        }
        else if(type==1){
            if(dpL[0]<k) return "-1";
            int v=0;
            while(1){
                int wv=(v==0)?0:cnt[v];
                if(k<=wv) return res;
                k-=wv;
                bool found=false;
                for(const auto&[c,u]:state[v].nxt){
                    if(k>dpL[u]) k-=dpL[u];
                    else{
                        res+=c;
                        v=u;
                        found=true;
                        break;
                    }
                }
                if(!found) return "-1";
            }
        }
        else{
            cerr<<"Query Type Error!"<<endl;
            exit(0);
        }
        return "-1";
    }

    //对外接口: 查询两个字符串的最长公共子串LCS
    //该接口属于 SAM 类本身,可直接调用 SAM::queryLCS(s,t) 进行LCS查询
    static pair<string,int> queryLCS(const string &s,const string &t){
        SAM sam(s);
        int lenLCS=0;
        int endpos=0;
        int v=0,l=0;
        for(int i=0;i<t.size();i++){
            char c=t[i];
            while(v!=-1&&!sam.state[v].nxt.count(c)){
                v=sam.state[v].link;
                l=sam.state[v].len;
            }
            if(v==-1){
                v=0;
                l=0;
                continue;
            }
            v=sam.state[v].nxt[c];
            l++;
            if(l>lenLCS){
                lenLCS=l;
                endpos=i;
            }
        }
        string LCS=t.substr(endpos-lenLCS+1,lenLCS);
        return {LCS,lenLCS};
    }

    //对外接口: 查询多个字符串的共同最长公共子串LCS
    //该接口属于 SAM 类本身,可直接调用 SAM::queryMultiLCS(str) 进行多串LCS查询
    static int queryMultiLCS(const vector<string>&str){
        string base;
        int minlen=INT_MAX/2;
        int idx;
        for(int i=0;i<str.size();i++){
            if(str[i].size()<minlen){
                base=str[i];
                minlen=str[i].size();
                idx=i;
            }
        }
        SAM sam(base);
        sam.buildOrd();
        vector<int>best(sam.siz+1);
        vector<int>LCSlen(sam.siz+1);
        for(int v=1;v<=sam.siz;v++) LCSlen[v]=sam.state[v].len;
        for(int i=0;i<str.size();i++){
            if(i==idx) continue;
            fill(best.begin(),best.end(),0);
            string t=str[i];
            int v=0,l=0;
            for(int i=0;i<t.size();i++){
                char c=t[i];
                while(v!=-1&&!sam.state[v].nxt.count(c)){
                    v=sam.state[v].link;
                    if(v!=-1) l=sam.state[v].len;
                }
                if(v==-1){
                    v=0;
                    l=0;
                    continue;
                }
                v=sam.state[v].nxt[c];
                l++;
                best[v]=max(best[v],l);
            }
            for(int v:sam.ord){
                int p=sam.state[v].link;
                if(p!=-1) best[p]=max(best[p],min(best[v],sam.state[p].len));
            }
            for(int v=1;v<=sam.siz;v++) LCSlen[v]=min(LCSlen[v],best[v]);
        }
        int res=*max_element(LCSlen.begin(),LCSlen.end());
        return res;
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    string s;
    cin>>s;
    SAM sam(s);
    sam.buildCnt();
    int ans=0;
    for(int v=1;v<=sam.siz;v++){
        if(sam.cnt[v]>1) ans=max(ans,sam.state[v].len*sam.cnt[v]);
    }
    cout<<ans;
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