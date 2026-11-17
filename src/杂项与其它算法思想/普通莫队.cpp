#include<bits/stdc++.h>
#define int long long int
#define TCASE 0

using namespace std;

/*
    模板例题: 此源代码(包括封装结构体模板)解决的是洛谷P2709
    1.普通莫队离线处理求解区间查询数字出现次数平方和: 洛谷P2709 Link: https://www.luogu.com.cn/problem/P2709
    2.普通莫队离线处理+简单组合数维护求解区间选数选中两个相同数的概率: 洛谷P1494 Link: https://www.luogu.com.cn/problem/P1494
    注: 莫队算法为一种离线处理的算法思想,为了更方便使用模板框架,此模板采用的是框架模板+策略类的方式实现
        即对于不同的可以运用普通莫队求解的问题,结构体 Mo_Normal 可以几乎不作更改
        对于不同问题,需自行自定义策略类 Policy 的成员变量及其维护方式
*/
template<class Policy> struct Mo_Normal{//普通莫队框架模板
    using Answer=typename Policy::Answer;
    struct Query{//查询存储
        int l,r,id;
    };
    int n,B;//序列长度,块的个数
    vector<Query>qry;
    Policy policy;//策略类

    // 构造函数,传入序列长度n和自定义维护策略
    explicit Mo_Normal(int n,Policy policy,int block_size=0):n(n),policy(move(policy)){
        B=block_size>0?block_size:max(1LL,(int)sqrt(n));
    }

    // 添加查询[l,r]
    void addQuery(int l,int r){
        qry.push_back({l,r,(int)qry.size()});
    }

    // 处理所有查询,返回答案数组
    vector<Answer> solve(){
        // 奇偶优化分块排序查询
        sort(qry.begin(),qry.end(),[&](const Query&a,const Query&b){
            int block_a=a.l/B,block_b=b.l/B;
            if(block_a!=block_b) return block_a<block_b;
            return (block_a&1)?(a.r>b.r):(a.r<b.r);
        });
        vector<Answer>ans(qry.size());
        int L=1,R=0;
        // 指针移动处理查询
        for(const auto&[l,r,id]:qry){
            // 区间扩展
            while(L>l) policy.add(--L);
            while(R<r) policy.add(++R);
            // 区间收缩
            while(L<l) policy.del(L++);
            while(R>r) policy.del(R--);
            ans[id]=policy.getAnswer();
        }
        return ans;
    }
};

struct Policy{//P2709策略类
    // 若一个数字x出现了c次,其对一次查询的答案贡献为c^2
    // 若查询区间变化时相对前一个查询加入一个x,出现次数变为c+1,答案变化Δ=(c+1)^2-c^2=2c+1
    // 因此对于add维护,ans+=2cnt[x]+1,cnt[x]++
    // 若查询区间变化时相对前一个查询删除一个x,出现次数变为c-1,答案变化Δ=(c-1)^2-c^2=1-2c
    // 因此对于del维护,ans-=2cnt[x]-1,cnt[x]--

    using Answer=int;
    vector<int>a;//原数组
    vector<int>cnt;//cnt[x]:x在当前区间出现的次数
    int ans=0;
    
    Policy(const vector<int>&a,int k):a(a){
        cnt.assign(k+1,0);
    }

    void add(int p){
        int x=a[p];
        ans+=2*cnt[x]+1;
        cnt[x]++;
    }

    void del(int p){
        int x=a[p];
        ans-=2*cnt[x]-1;
        cnt[x]--;
    }

    Answer getAnswer(){
        return ans;
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n,m,k;
    cin>>n>>m>>k;
    vector<int>a(n+1);
    for(int i=1;i<=n;i++) cin>>a[i];
    Mo_Normal<Policy>mo(n,Policy(a,k));
    for(int i=0;i<m;i++){
        int l,r;
        cin>>l>>r;
        mo.addQuery(l,r);
    }
    auto ans=mo.solve();
    for(auto x:ans) cout<<x<<endl;
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