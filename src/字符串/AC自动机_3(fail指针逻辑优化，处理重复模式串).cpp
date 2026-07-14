/*Coding by YOLU_gargaring*/
#include<bits/stdc++.h>
#define int long long int
#define db long double
#define endl '\n'
#define pii pair<int,int>
#define tri tuple<int,int,int>
#define prque priority_queue 
#define all(x) x.begin(),x.end()
#define MX INT_MAX
#define MN INT_MIN
#define E 1e-6
#define st_it set<int>::iterator
#define mst_it multiset<int>::iterator
#define list_it list<int>::iterator
#define itr ::iterator

using namespace std;

const int MAXN=2e5+5;
const int MAXM=2e5+5;
const int z=26;

class ACauto {
private:
    int node[MAXN][z];
    int fail[MAXN];
    int in[MAXN];
    int tot;           
public:
    int cnt[MAXM];
    int pos[MAXM];
    void ReACauto() {
        for(int i=0;i<=tot;i++){
            memset(node[i],0,sizeof(node[i]));
            fail[i]=in[i]=cnt[i]=0;
        }
        tot=0;
        memset(pos,0,sizeof(pos));
    }
    
    void insert(string word,int idx) {
        int u=0;
        for(char c:word){
            int v=c-'a';
            if(!node[u][v]){
                tot++;
                node[u][v]=tot;
            }
            u=node[u][v];
        }
        pos[idx]=u;
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
                    in[fail[node[u][i]]]++;
                    q.push(node[u][i]);
                }
                else node[u][i]=node[fail[u]][i];
            }
        }
    }
    void query(string text) {
        int u=0;
        for(char c:text){
            int v=c-'a';
            u=node[u][v];
           cnt[u]++;
        }
    }
    void topoSort(){
        queue<int>q;
        for(int i=1;i<=tot;i++){
            if(in[i]==0) q.push(i);
        }
        while(!q.empty()){
            int u=q.front();
            q.pop();
            cnt[fail[u]]+=cnt[u];
            in[fail[u]]--;
            if(in[fail[u]]==0) q.push(fail[u]);
        }
    }
};

void solve() {
    int n;
    cin>>n;
    ACauto ac;
    ac.ReACauto();
    for(int i=1;i<=n;i++){
        string s;
        cin>>s;
        ac.insert(s,i);
    }
    string t;
    cin>>t;
    ac.build();
    ac.query(t);
    ac.topoSort();
    for(int i=1;i<=n;i++) cout<<ac.cnt[ac.pos[i]]<<endl;
} 
 
signed main(){
    ios::sync_with_stdio(0);
    cin.tie(nullptr);
    int T;
    //cin>>T;
	   T=1;
       int n;
	  while(T--) solve();
    return 0;
}