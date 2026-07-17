#include<bits/stdc++.h>
#define int long long int
#define endl '\n'
// #define mod (int)(998244353)
#define mod (int)(1e9+7)
#define Tcase 0

using namespace std;

/*
    模板原题: 此源代码的solve()解决的是 洛谷P3812
    1.原数组子序列最大异或和: 洛谷P3812 Link: https://www.luogu.com.cn/problem/P3812
    2.查询前缀子序列异或和结果为x的方案数: CF959F Link: https://codeforces.com/problemset/problem/959/F
    3.区间子序列最大异或和: CF1100F Link: https://codeforces.com/problemset/problem/1100/F
    4.原数组子序列异或和第K小: HDU3949 Link: https://vjudge.net/problem/HDU-3949
*/

struct XOR_Basis{//线性基,用最少的数字表示异或空间
    const static int H=61;
    // const static int H=32;
    vector<int>d;//d[i]:当前线性基中最高位为i的代表元素
    vector<int>pos;//pos[i]:该基向量最晚来自哪个位置,最高位i的方向来自第几个元素
    int cnt;//线性基的秩(人话,插入到线性基中被有效保留的数的个数)

    XOR_Basis():d(H),pos(H),cnt(0){}
    XOR_Basis(int n):d(n),pos(n),cnt(0){}//有参构造,n取所有插入元素的最大值的二进制位数

    bool insert(int x){//将x插入当前线性基
        for(int i=d.size()-1;i>=0;i--){
            if((x>>i)&1){
                if(d[i]==0){
                    d[i]=x;
                    cnt++;
                    return true;
                }
                x^=d[i];
            }
        }
        return false;
    }

    bool insert(int x,int id){//将x插入当前线性基(带位置,用于区间异或查询的插入)
        for(int i=d.size()-1;i>=0;i--){
            if((x>>i)&1){
                if(d[i]==0){
                    d[i]=x;
                    cnt++;
                    pos[i]=id;
                    return true;
                }
                if(pos[i]<id){
                    swap(pos[i],id);
                    swap(d[i],x);
                }
                x^=d[i];
            }
        }
        return false;
    }

    bool canXorTo(int x){//查询给定数x是否能被当前线性基表示,即x是否可以用当前线性基中的数表示出来
        for(int i=d.size()-1;i>=0;i--){
            if((x>>i)&1){
                if(d[i]==0) return false;
                x^=d[i];
            }
        }
        return true;
    }

    void rebuild(){//重构线性基以便K-th异或查询
        for(int i=0;i<d.size();i++){
            for(int j=i-1;j>=0;j--){
                if((d[i]>>j)&1)
                    d[i]^=d[j];
            }
        }
        int id=0;
        for(int i=0;i<d.size();i++){
            if(d[i]){
                swap(d[id],d[i]);
                id++;
            }
        }
    }

    int queryMax(){//查询当前线性基所能异或出的最大值
        int res=0;
        for(int i=d.size()-1;i>=0;i--) res=max(res,res^d[i]);
        return res;
    }

    int queryMax(int L){//查询当前线性基对于原数组排除位置L之前的数所能异或出的最大值(通常用于区间异或查询)
        int res=0;
        for(int i=d.size()-1;i>=0;i--){
            if(pos[i]>=L) res=max(res,res^d[i]);
        }
        return res;
    }

    int queryCount(int x,int n){//查询当前长度为n的数组/前缀的子序列XORSUM=x的子序列个数
        if(canXorTo(x)) return 1LL<<(n-cnt);//方案数为2^(n-cnt),根据题目要求可在此处改为带模快速幂
        return 0;
    }

    int queryKth(int k,int n){//查询当前线性基能异或出的第k小的值,n:原数组大小
        bool canZero=(cnt<n);//秩小于数组大小,说明原数组存在线性相关的数,XOR第一小为0
        int all=(1LL<<cnt)-(canZero?0:1);//子序列可以异或出的结果总数
        if(k>all) return -1;
        if(canZero) k--;
        int res=0;
        for(int i=0;i<cnt;i++){
            if((k>>i)&1) res^=d[i];
        }
        return res;
    }

    void copy(vector<int>&a){//线性基复制,将另一个线性基的d[]复制进一个空线性基
        for(int &x:a) insert(x);
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n;
    cin>>n;
    XOR_Basis xbs;
    //将序列的所有数插入线性基,线性基会排除一些可以用已有元素异或表示的数
    //直接调用queryMax()即可求出序列中选元素的最大异或和
    rep1(i,1,n){
        int x;
        cin>>x;
        xbs.insert(x);
    }
    cout<<xbs.queryMax();
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