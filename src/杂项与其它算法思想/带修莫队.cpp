#include<bits/stdc++.h>
#define int long long int
#define TCASE 0

using namespace std;

/*
    模板例题: 此源代码(包括封装结构体模板)解决的是洛谷P1903
    1.带修莫队求解带单点颜色修改的区间颜色数查询: 洛谷P1903 Link: https://www.luogu.com.cn/problem/P1903
    2.带修莫队+离散化求解带单点修改的区间中等于K的值的个数查询 洛谷P2464 Link: https://www.luogu.com.cn/problem/P2464
    注: 莫队算法为一种离线处理的算法思想,为了更方便使用模板框架,此模板采用的是框架模板+策略类的方式实现
        即对于不同的可以运用带修莫队求解的问题,结构体 Mo_Modifiable 可以几乎不作更改
        对于P2464这类带有第三参数的区间查询问题需要修改 Mo_Modifiable 的查询存储结构体构成和solve()的细节
        对于不同问题,需自行自定义策略类 Policy 的成员变量及其维护方式
*/
template<class Policy> struct Mo_Modifiable{//带修莫队框架模板
    using Answer=typename Policy::Answer;
    struct Query{//查询存储
        int l,r,id;//左右端点,查询编号
        int t;//修改操作时间戳
    };
    struct Operation{//单点修改操作存储
        int p;//位置
        int val;//修改的值
    };
    int n,B;//序列长度,块的个数
    vector<Query>qry;
    vector<Operation>ops;
    Policy policy;//策略类

    // 构造函数,传入序列长度n和自定义维护策略
    explicit Mo_Modifiable(int n,Policy policy,int block_size=0):n(n),policy(move(policy)){
        B=block_size>0?block_size:max(1LL,(int)pow(n,2.0/3.0));
    }

    // 添加查询[l,r]
    void addQuery(int l,int r){
        qry.push_back({l,r,(int)qry.size(),(int)ops.size()});
    }

    // 添加单点修改
    void addModify(int p,int x){
        ops.push_back({p,x});
    }

    // 处理所有查询,返回答案数组
    vector<Answer> solve(){
        // 按左块,右块,时间戳三关键字排序查询
        sort(qry.begin(),qry.end(),[&](const Query&a,const Query&b){
            int block_la=a.l/B,block_lb=b.l/B;
            if(block_la!=block_lb) return block_la<block_lb;
            int block_ra=a.r/B,block_rb=b.r/B;
            if(block_ra!=block_rb) return block_ra<block_rb;
            return a.t<b.t;
        });
        vector<Answer>ans(qry.size());
        int L=1,R=0,T=0;
        // 指针移动处理查询
        for(const auto&[l,r,id,t]:qry){
            // 区间扩展
            while(L>l) policy.add(--L);
            while(R<r) policy.add(++R);
            // 区间收缩
            while(L<l) policy.del(L++);
            while(R>r) policy.del(R--);
            // 时间前进执行修改
            while(T<t){
                auto&[p,val]=ops[T];
                policy.applyModify(p,val,L,R);
                T++;
            }
            // 时间回退撤销修改
            while(T>t){
                T--;
                auto&[p,val]=ops[T];
                policy.applyModify(p,val,L,R);
            }
            ans[id]=policy.getAnswer();
        }
        return ans;
    }
};

struct Policy{//P1903策略类
    const static int N=1e6+5;
    using Answer=int;
    vector<int>a;//原数组
    vector<int>cnt;//cnt[x]:颜色x在当前区间出现的次数
    int ans=0;
    
    Policy(const vector<int>&a):a(a){
        cnt.assign(N,0);
    }

    void add(int p){
        int x=a[p];
        if(++cnt[x]==1) ans++;
    }

    void del(int p){
        int x=a[p];
        if(--cnt[x]==0) ans--;
    }

    void applyModify(int &p,int &v,int L,int R){
        if(L<=p&&p<=R) del(p);
        swap(a[p],v);
        if(L<=p&&p<=R) add(p);
    }

    Answer getAnswer(){
        return ans;
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n,m;
    cin>>n>>m;
    vector<int>a(n+1);
    for(int i=1;i<=n;++i) cin>>a[i];
    Mo_Modifiable<Policy>mo(n,Policy(a));
    while(m--){
        char op;
        int x,y;
        cin>>op>>x>>y;
        if(op=='Q') mo.addQuery(x,y);
        else mo.addModify(x,y);
    }
    auto ans=mo.solve();
    for(const auto&x:ans) cout<<x<<endl;
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