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
    5.单次查询字典序第K小子串-分位置不同和本质不同两种情况: 洛谷P3975 Link: https://www.luogu.com.cn/problem/P3975
    6.多次查询字典序第K小子串-本质不同情况: SPOJ7258 Link: https://vjudge.net/problem/SPOJ-SUBLEX#author=main
    7.字符串拼接式伪广义SAM求三个字符串的公共子串出现位置三元组组合数: CF452E Link: https://codeforces.com/contest/452/problem/E
    8.查询不包含某个字符的本质不同子串个数: ABC452G Link: https://atcoder.jp/contests/abc452/tasks/abc452_g
*/
struct SAM{//后缀自动机
    struct node{//状态节点
        int len;//该状态对应的最长字串长度
        int link;//该状态的后缀链接
        map<char,int>nxt;//转移列表
        vector<int>gcnt;//当前状态里所有子串在每个字符串中的出现次数(gcnt[i]对应第i个字符串),用于字符串拼接的伪广义SAM
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
    int GSAMstr;//字符串拼接式伪广义SAM所用到的字符串个数
    bool ordBuilt;//标记是否已经构建了按len[v]从大大小排序的状态序列
    bool cntBuilt;//标记是否已经完成了查询子串出现次数所需的cnt构建
    bool dpBuilt;//标记是否已经完成了查询字典序第k小子串所需的DP构建
    bool gcntBuilt;//标记是否已经完成了查询子串在每个字符串的出现次数所需的gcnt构建
    const string KthSubNotfound="No such line.";//找不到第k小子串输出信息,可直接在此处修改

    SAM(){}

    SAM(string s){//构造字符串s的SAM
        int n=s.size();
        initEmpty(n,0);
        for(const auto&c:s) extend(c);
    }

    SAM(vector<string> str){//构造字符串数组str和分隔符拼接的伪广义SAM,str大小不能超过64
        int m=str.size();
        int totL=m;
        for(int i=0;i<m;i++) totL+=str[i].size();
        initEmpty(totL,m);
        for(int i=0;i<m;i++){
            for(const auto&c:str[i]) extend(c,i);
            extend(char(i+1));
        }
    }

    void initEmpty(int n,int m){//构造只有初始节点的空SAM,m为构建字符串拼接式伪广义SAM的字符串个数,不构建伪广义SAM可传入0
        maxn=2*n+1;
        GSAMstr=m;
        state.assign(maxn,{0,0,{},vector<int>(m,0)});
        cnt.assign(maxn,0);
        state[0].len=0;
        state[0].link=-1;
        siz=1;
        last=0;
        differentSub=0;
        ordBuilt=false;
        cntBuilt=false;
        gcntBuilt=false;
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
        ordReset();
        return cur;
    }

    int extend(char c,int id){//带编号插入字符,用于构建字符串拼接式伪广义SAM
        int cur=extend(c);
        state[cur].gcnt[id]++;
        return cur;
    }

    //字符串末尾新增字符后状态更新,再次调用buildOrd,buildCnt,buildDP需要重置ord,cnt,dp
    void ordReset(){
        ordBuilt=false;
        cntBuilt=false;
        gcntBuilt=false;
        dpBuilt=false;
    }

    void buildOrd(){//构造按len降序排列的状态序列
        if(ordBuilt) return;
        ord.assign(siz,0);
        iota(ord.begin(),ord.end(),0);
        sort(ord.begin(),ord.end(),[&](int x,int y){return state[x].len<state[y].len;});
        reverse(ord.begin(),ord.end());
        ordBuilt=true;
    }

    void buildCnt(){//统计每个状态子串出现次数
        if(cntBuilt) return;
        buildOrd();
        for(int v:ord){
            if(state[v].link!=-1) cnt[state[v].link]+=cnt[v];
        }
        cntBuilt=true;
    }

    void buildGcnt(){//统计每个状态对应的子串在每个字符串中的出现次数
        if(gcntBuilt) return;
        buildOrd();
        for(int v:ord){
            if(state[v].link!=-1){
                for(int i=0;i<GSAMstr;i++) state[state[v].link].gcnt[i]+=state[v].gcnt[i];
            }
        }
        gcntBuilt=true;
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

    int querySub_withoutBan(char ban){//查询当前所有子串中不包含字符ban的本质不同子串个数
        buildOrd();
        vector<int>tdp(siz);
        for(int v:ord){
            int sum=0;
            for(const auto&[c,u]:state[v].nxt){
                if(c==ban) continue;
                sum+=(1+tdp[u]);
            }
            tdp[v]=sum;
        }
        return tdp[0];
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
    //若有多次s的第k小子串查询推荐显式构造 SAM sam(s) 后调用此接口 sam.queryKthSub(k,type)
    string queryKthSub(int k,int type){
        string res="";
        buildDP();
        if(type==0){
            if(dp[0]<k) return KthSubNotfound;
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
                if(!found) return KthSubNotfound;
            }
        }
        else if(type==1){
            if(dpL[0]<k) return KthSubNotfound;
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
                if(!found) return KthSubNotfound;
            }
        }
        else{
            cerr<<"Query Type Error!"<<endl;
            exit(0);
        }
        return KthSubNotfound;
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

    //对外接口: 查询字符串的 本质不同第K小字典序子串(参数type=0) 或 查询位置不同第K小字典序子串(参数type=1)
    //该接口属于 SAM 类本身,可直接调用 SAM::queryKthSub(s,k,type) 进行s的字典序第k小子串查询
    //若只有单次s的查询第k小子串推荐调用此接口
    static string queryKthSub(string s,int k,int type){
        if(type!=0&&type!=1){
            cerr<<"Query Type Error!"<<endl;
            exit(0);
        }
        SAM sam(s);
        string res=sam.queryKthSub(k,type);
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