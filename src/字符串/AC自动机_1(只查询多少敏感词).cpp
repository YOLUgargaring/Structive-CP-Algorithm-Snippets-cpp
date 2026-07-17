#include<bits/stdc++.h>
#define int long long int

using namespace std;

//模板原题: 洛谷P3808 Link: https://www.luogu.com.cn/problem/P3808
class ACauto {
    const static int MAXN=1e6+5;
    const static int z=26;
private:
    int node[MAXN][z];
    int fail[MAXN];
    int end[MAXN];
    int tot;           
public:
    ACauto() {
        tot=0;
        memset(node,0,sizeof(node));
        memset(fail,0,sizeof(fail));
        memset(end,0,sizeof(end));
    }
    void insert(string word) {
        int u=0;
        for(char c:word){
            int v=c-'a';
            if(!node[u][v]){
                tot++;
                node[u][v]=tot;
            }
            u=node[u][v];
        }
        end[u]++;
    }
    void build() {
        queue<int>q;
        for(int i=0;i<z;i++){
            if(node[0][i]){
                fail[node[0][i]]=0;
                q.push(node[0][i]);
            }
        }
        while(!q.empty()){
            int u=q.front();
            q.pop();
            for(int i=0;i<z;i++){
                if(node[u][i]){
                    fail[node[u][i]]=node[fail[u]][i];
                    q.push(node[u][i]);
                }
                else node[u][i]=node[fail[u]][i];
            }
        }
    }
    int query(string text) {
        int u=0;
        int res=0;
        for(char c:text){
            int v=c-'a';
            u=node[u][v];
            int tmp=u;
            while(tmp!=0&&end[tmp]!=-1){
                res+=end[tmp];
                end[tmp]=-1;
                tmp=fail[tmp];
            }
        }
        return res;
    }
};

void solve() {
    int n;
    cin>>n;
    ACauto ac;
    for(int i=0;i<n;i++){
        string s;
        cin>>s;
        ac.insert(s);
    }
    ac.build();
    string t;
    cin>>t;
    cout<<ac.query(t);
} 
 
signed main(){
    ios::sync_with_stdio(0);
    cin.tie(nullptr);
    int T;
    //cin>>T;
	   T=1;
	  while(T--) solve();
    return 0;
}