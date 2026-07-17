#include<bits/stdc++.h>
#define int long long int
#define endl '\n'
#define Tcase 0

using namespace std;

//模板原题: 洛谷P3865 Link: https://www.luogu.com.cn/problem/P3865
struct ST{
    const static int M=200001;
	const static int lgN=21;
    int n;
    vector<int>logn;
	vector<vector<int>>mx;//区间最大值
	vector<vector<int>>mn;//区间最小值
    ST(int n): n(n),logn(M),mx(n+1,vector<int>(lgN+1,0)),mn(n+1,vector<int>(lgN+1,0)){}
    void pre(){
        logn[1]=0;
        logn[2]=1;
        for(int i=3;i<M;i++) logn[i]=logn[i/2]+1;
    }//logn预处理
    void init(vector<int>&nums){
        pre();
        for(int i=1;i<=n;i++){
            mx[i][0]=mn[i][0]=nums[i-1];
        }
        for(int j=1;j<=lgN;j++){
            for(int i=1;i+(1<<j)-1<=n;i++){
                mx[i][j]=max(mx[i][j-1],mx[i+(1<<(j-1))][j-1]);
                mn[i][j]=min(mn[i][j-1],mn[i+(1<<(j-1))][j-1]);
            }
        }
    }//造表,1-based	
    int queryMax(int l,int r){//区间max,1-based
        int s=logn[r-l+1];
		int maxlr=max(mx[l][s],mx[r-(1<<s)+1][s]);
        return maxlr;
    }
    int queryMin(int l,int r){//区间min,1-based
        int s=logn[r-l+1];
        int minlr=min(mn[l][s],mn[r-(1<<s)+1][s]);
        return minlr;
    }
    int queryMinMaxDiff(int l,int r){//区间max-min,1-based
        return queryMax(l,r)-queryMin(l,r);
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n,m;
    cin>>n>>m;
    vector<int>a(n);
    for(int &x:a) cin>>x;
    ST st(n);
    st.init(a);
    while(m--){
        int l,r;
        cin>>l>>r;
        cout<<st.queryMax(l,r)<<endl;
    }
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