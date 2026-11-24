#include<bits/stdc++.h>
#define TCASE 0

using namespace std;

/*
    模板例题: 此源代码的solve()解决的是洛谷P3370
    1.字符串哈希模板-字符串种类数求解: 洛谷P3370 Link: https://www.luogu.com.cn/problem/P3370
    2.字符串哈希求解恰好只有一位字符不同的字符串对数: 洛谷P4503 Link: https://www.luogu.com.cn/problem/P4503
    3.字符串哈希+枚举求解相差一个字符的字符串个数查询: 洛谷P4407 Link: https://www.luogu.com.cn/problem/P4407
    4.字符串哈希+二分答案求解01字符串的反对称子串个数: 洛谷P3501 Link: https://www.luogu.com.cn/problem/P3501
*/
struct StringHash{//字符串哈希-自然溢出哈希
    using ull=unsigned long long;
    const static ull P=131;
    int n;//原字符串长度
    vector<ull>h;//h[i]:以第i个字符结尾的前缀哈希值
    vector<ull>p;//p[i]:p^i

    // 构造,传入0-based字符串
    StringHash(const string&s){
        n=s.size();
        h.assign(n+1,0);
        p.assign(n+1,0);
        p[0]=1;
        for(int i=1;i<=n;i++){
            h[i]=h[i-1]*P+singleHash(s[i-1]);
            p[i]=p[i-1]*P;
        }
    }

    static ull singleHash(char c){
        return static_cast<unsigned char>(c)+1;
    }

    // 查询子串[l,r]的哈希值,1-based
    ull queryRangeHash(int l,int r) const{
        return h[r]-h[l-1]*p[r-l+1];
    }

    // 查询删除第k个字符后的字符串哈希值,1-based
    ull queryDeletePHash(int k) const{
        // 设一个字符串S的哈希值为H(S),有H(A+B)=H(A)P^|B|+H(B)
        ull h_left=(k==1)?0:h[k-1];
        ull h_righ=(k==n)?0:queryRangeHash(k+1,n);
        return h_left*p[n-k]+h_righ;
    }

    // 查询替换第k个字符为c后的字符串哈希值,1-based
    ull queryReplacePHash(int k,char c) const{
        // 设一个字符串S的哈希值为H(S),单个字符c的哈希值为v(c)
        // 第k个字符替换为c后有: H(S')=H(S)-v(S[k])·P^(n-k)+v(c)·P^(n-k)
        ull old_v=queryRangeHash(k,k);
        ull new_v=singleHash(c);
        return h[n]+(new_v-old_v)*p[n-k];
    }

    // 查询在第k个字符前插入字符c后的字符串哈希值,1-based,k∈[1,n+1]
    ull queryInsertPHash(int k,char c) const{
        // 设一个字符串S的哈希值为H(S),单个字符c的哈希值为v(c)
        // 第k个字符之前插入c后有: H(S')=(H(S[:k-1])·P+v(c))·P^(n-k+1)+H(S[k:n])
        ull h_left=h[k-1];
        ull h_right=queryRangeHash(k,n);
        ull insv=singleHash(c);
        return (h_left*P+insv)*p[n-k+1]+h_right;
    }

    // 查询该字符串哈希值
    ull queryHash() const{
        return h[n];
    }

    // 查询子串[l1,r1]和子串[l2,r2]是否一样,1-based
    bool querySubEqual(int l1,int r1,int l2,int r2) const{
        if(r1-l1!=r2-l2) return false;
        return queryRangeHash(l1,r1)==queryRangeHash(l2,r2);
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n;
    cin>>n;
    unordered_set<unsigned long long>st;
    while(n--){
        string s;
        cin>>s;
        StringHash hsh(s);
        st.insert(hsh.queryHash());
    }
    cout<<st.size();
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