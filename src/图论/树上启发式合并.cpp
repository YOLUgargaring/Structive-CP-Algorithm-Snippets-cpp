#include<bits/stdc++.h>
#define int long long int
#define TCASE 0

using namespace std;

/*
    模板例题: 此源代码(包括封装化结构体模板)解决的是洛谷P9233
    1.树上启发式合并求解颜色出现次数相同的节点数相同的子树总数:
        洛谷P9233&蓝桥杯2023省A真题 Link: https://www.luogu.com.cn/problem/P9233
    2.树上启发式合并优化树上子树相差值最少的差值之和与多重集维护贡献:
        洛谷P13900&CCF-CSP028-T5 Link: https://www.luogu.com.cn/problem/P13900
    注: 树上启发式合并是一种树上信息维护与处理的算法思想,不存在标准模板,此模板仅提供一定参考,可套用类似框架
*/
struct HMT{//树上启发式合并Heuristic Merge on Tree(DSU on Tree容易误导为要用并查集,所以采用直译名)
    int n,s;//点数,根节点
    vector<vector<int>>tr;//邻接表图
    vector<int>sz;//sz[u]:u的子树大小
    vector<int>big;//big[u]:u的重儿子,子节点中子树大小最大的
    // 维护信息
    // P9233,统计每种颜色的节点个数都相同的子树的个数
    vector<int>col;//cnt[u]:节点u的颜色
    vector<int>cnt;//cnt[c]:当前统计范围内颜色c出现的次数
    vector<int>freq;//freq[k]:当前统计范围内出现次数为k的颜色树
    int dif;//当前统计范围内有多少k满足freq[k]>0,dif=1时则该子树合法,ans+1
    int ans;//每种颜色的节点个数都相同的子树的个数

   HMT(int n,int s):n(n),s(s),tr(n+1),sz(n+1),big(n+1){
        // 其余根据问题的待处理项↓
        col.assign(n+1,0);
        cnt.assign(n+1,0);
        freq.assign(n+1,0);
        dif=0;
        ans=0;
    }

    // 添树边u-v
    void add(int u,int v){
        tr[u].push_back(v);
    }

    // 预处理每个节点的重儿子,树上启发式合并基本操作
    void dfsi(int u){
        sz[u]=1;
        for(int v:tr[u]){
            dfsi(v);
            sz[u]+=sz[v];
            if(sz[v]>sz[big[u]]) big[u]=v;
        }
    }

    // 维护信息处理↓
    void setCol(int u,int c){//节点u染色c
        col[u]=c;
    }

    void modify(int c,int d){//加入/删除一个颜色为c的节点,d为1加入,为-1删除
        // 若颜色为c的节点原本出现了x次,则颜色c的出现次数将变为x+1
        // 此时freq[x+1]++,若x>0,则freq[x]--
        // 若某频次的颜色数从1变为0,则dif--,若某频次的颜色数从0变为1,则dif++
        // 删除一个颜色c的节点反向进行类似操作
        int x=cnt[c];
        if(x>0){
            freq[x]--;
            if(freq[x]==0) dif--;
        }
        cnt[c]+=d;
        int nx=cnt[c];
        if(nx>0){
            freq[nx]++;
            if(freq[nx]==1) dif++;
        }
    }

    void modifySub(int u,int d){//更新u的子树信息
        modify(col[u],d);
        for(int v:tr[u]) modifySub(v,d);
    }
    // 维护信息处理↑

    // 根据问题定义的树上启发式合并核心DFS,keep决定处理节点u的子树后是否保留该子树的维护信息
    void dfs(int u,bool keep){
        // 先处理节点u的轻儿子dfs(v,false),计算轻子树答案,结束后清除贡献
        // 再处理节点u的重儿子dfs(big[u],true),计算重子树答案,保留贡献
        // 接着合并当前节点u,重新加入轻儿子贡献,更新维护的总答案
        // 最后若keep==false则删除u的子树的贡献,否则保留
        for(int v:tr[u]){
            if(v!=big[u]) dfs(v,false);
        }
        if(big[u]!=0) dfs(big[u],true);
        for(int v:tr[u]){
            if(v!=big[u]){
                // 加入轻儿子贡献
                modifySub(v,1);
            }
        }
        // 合并当前节点u与更新维护的答案
        modify(col[u],1);
        if(dif==1) ans++;
        if(!keep){
            // 清除子树贡献
            modifySub(u,-1);
        }
    }

    // 根据问题跑树上启发式合并
    void solve(){
        dfsi(s);
        dfs(s,1);
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n;
    cin>>n;
    HMT hmt(n,1);
    for(int i=1;i<=n;i++){
        int c,f;
        cin>>c>>f;
        hmt.setCol(i,c);
        if(f!=0) hmt.add(f,i);
    }
    hmt.solve();
    cout<<hmt.ans;
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