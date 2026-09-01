#include<bits/stdc++.h>
#define int long long int
#define rep1(i,l,r) for(int i=l;i<=r;i++)
#define TCASE 0

using namespace std;
/*
    模板原题: 此源代码的solve()解决的是CF896C
    1.珂朵莉树出处原题-区间加区间赋值区间第k小区间次幂和查询: CF896C Link: https://codeforces.com/problemset/problem/896/C
    2.区间不同值个数查询: 洛谷P1558 Link: https://www.luogu.com.cn/problem/P1558
    3.查询区间中的值为v的个数后区间推平: 洛谷P13983 Link: https://www.luogu.com.cn/problem/P13983
*/
class ChthollyTree{//珂朵莉树/颜色段均摊
private:
    struct Node{//区间节点,相同值的区间视为一个节点
        int l,r;//区间边界
        mutable int v;//区间内的值,可变
        bool operator<(const Node&o)const{return l<o.l;}
    };
    int n;//区间总长度
    int t=-1;//不同值个数上限
    set<Node>st;//节点池

    //区间分裂:将pos所在的区间[l,r]分裂为两个区间[l,pos)和[pos,r],返回右侧区间的迭代器
    set<Node>::iterator split(int pos){
        if(pos>n) return st.end();
        auto it=st.lower_bound({pos,0,0});
        if(it!=st.end()&&it->l==pos) return it;
        --it;
        int l=it->l,r=it->r,v=it->v;
        st.erase(it);
        st.insert({l,pos-1,v});
        return st.insert({pos,r,v}).first;//返回右侧区间的迭代器,即右侧区间左端点
    }

public:
    void build(const vector<int>&a){//对1-based原数组a构建初始珂朵莉树
        n=a.size()-1;
        int left=1;
        for(int i=1;i<=n;i++){
            if(i==n||a[i]!=a[i+1]){
                st.insert({left,i,a[i]});
                left=i+1;
            }
        }
    }

    void setDifferentLimit(int limit){//设置不同值个数上限,用于优化queryDifferent
        t=limit;
    }

    void RangeAssign(int l,int r,int v){//区间[l,r]全赋值v
        auto itr=split(r+1),itl=split(l);//切分边界
        st.erase(itl,itr);//删除旧颜色段区间
        st.insert({l,r,v});//插入新区间
    }

    void RangeAdd(int l,int r,int v){//区间[l,r]全加v
        auto itr=split(r+1),itl=split(l);//切分边界
        for(auto it=itl;it!=itr;it++) it->v+=v;//遍历区间,加上v
    }

    int queryKth(int l,int r,int k){//查询区间[l,r]第k小值
        auto itr=split(r+1),itl=split(l);//切分边界
        vector<pair<int,int>>vec;//{值,出现次数}
        for(auto it=itl;it!=itr;it++) vec.push_back({it->v,it->r-it->l+1});//统计区间内的值及其出现次数
        sort(vec.begin(),vec.end());//按值排序
        for(auto [v,cnt]:vec){
            if(k<=cnt) return v;//第k小值在当前值的区间内
            k-=cnt;//否则减去当前值的出现次数,继续寻找
        }
        return -1;//如果k大于区间内的总元素个数,返回-1表示不存在
    }

    static int qpow(int a,int b, int p){//带模快速幂计算a^b mod p
        int res=1;
        int base=a;
        while(b){
            if(b&1){
                res%=p;
                res*=base;
                res%=p;
            }
            base%=p;
            base*=base;
            base%=p;
            b>>=1;
        }    
        return res;
    }

    int querySum(int l,int r,int p){//查询区间[l,r]的和 mod p
        auto itr=split(r+1),itl=split(l);//切分边界
        int sum=0;
        for(auto it=itl;it!=itr;it++){
            int len=it->r-it->l+1;
            sum=(sum+len*it->v)%p;//累加区间和,并取模
        }
        return sum;
    }

    int queryPowSum(int l,int r,int x,int p){//查询区间[l,r]的x次幂和 mod p
        auto itr=split(r+1),itl=split(l);//切分边界
        int sum=0;
        for(auto it=itl;it!=itr;it++){
            int len=it->r-it->l+1;
            sum=(sum+len*qpow(it->v,x,p))%p;//累加区间x次幂和,并取模
        }
        return sum;
    }

    int queryDifferent(int l,int r){//查询区间[l,r]的不同值个数
        auto itr=split(r+1),itl=split(l);//切分边界
        bitset<60>bs;//使用位集记录不同值
        for(auto it=itl;it!=itr;it++){
            bs.set(it->v);
            if(bs.count()==t) return t;
        }
        return bs.count();//返回不同值个数
    }

    int queryCount(int l,int r,int v){//查询区间[l,r]中值为v的个数
        auto itr=split(r+1),itl=split(l);//切分边界
        int cnt=0;
        for(auto it=itl;it!=itr;it++){
            if(it->v==v) cnt+=it->r-it->l+1;//累加值为v的区间长度
        }
        return cnt;
    }
};

struct Query{
    int op;
    int l,r;
    int x,y;
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n,m,seed,vmax;
    cin>>n>>m>>seed>>vmax;
    vector<int>a(n+1);
    vector<Query>qrs;
    auto rnd=[&]()->int {
        int ret=seed;
        seed=(seed*7+13)%1000000007;
        return ret;
    };
    auto init=[&]()->void {
        rep1(i,1,n) a[i]=rnd()%vmax+1;
        rep1(i,1,m){
            int op=rnd()%4+1;
            int l=rnd()%n+1,r=rnd()%n+1;
            if(l>r) swap(l,r);
            int x,y;
            if(op==3) x=rnd()%(r-l+1)+1;
            else x=rnd()%vmax+1;
            if(op==4) y=rnd()%vmax+1;
            qrs.push_back({op,l,r,x,y});
        }
    };
    init();
    ChthollyTree odt;
    odt.build(a);
    for(auto [op,l,r,x,y]:qrs){
        if(op==1) odt.RangeAdd(l,r,x);
        else if(op==2) odt.RangeAssign(l,r,x);
        else if(op==3) cout<<odt.queryKth(l,r,x)<<endl;
        else if(op==4) cout<<odt.queryPowSum(l,r,x,y)<<endl;
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