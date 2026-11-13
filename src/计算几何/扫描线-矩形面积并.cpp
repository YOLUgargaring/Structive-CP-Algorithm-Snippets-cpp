#include<bits/stdc++.h>
#define int long long int
#define TCASE 0

using namespace std;

// 模板原题: 洛谷P5490 Link:https://www.luogu.com.cn/problem/P5490
struct Event{//从左往右扫描线事件
    int x,y1,y2;
    int k;// 左边界+1,右边界-1
};

struct SegTree{//线段树维护区间覆盖-区间覆盖次数±1/查询整个范围内至少被覆盖一次的总区间并长度
    int n;
    vector<int>cnt;//cnt[p]:区间更新时,记录在该节点的覆盖次数
    vector<int>len;//len[p]:由该节点及其子树的覆盖记录产生的区间并长度
    vector<int>y;

    SegTree(const vector<int>&ys){
        y=ys;
        n=y.size()-1;
        cnt.assign((n<<2)+5,0);
        len.assign((n<<2)+5,0);
    }

    void pushup(int p,int l,int r){
        if(cnt[p]>0) len[p]=y[r+1]-y[l];
        else if(l==r) len[p]=0;
        else len[p]=len[p<<1]+len[p<<1|1];
    }

    void updateR(int p,int l,int r,int ul,int ur,int k){
        if(ul<=l&&ur>=r){
            cnt[p]+=k;
            pushup(p,l,r);
            return;
        }
        int mid=l+(r-l)/2;
        if(ul<=mid) updateR(p<<1,l,mid,ul,ur,k);
        if(ur>mid) updateR(p<<1|1,mid+1,r,ul,ur,k);
        pushup(p,l,r);
    }
    void updateR(int l,int r,int k){updateR(1,0,n-1,l,r,k);}

    int query(){//查询所有至少被覆盖一次的区间的并长度,根节点len[1]即为结果
        return len[1];
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n;
    cin>>n;
    vector<int>y(n<<1);
    vector<Event>evt(n<<1);
    for(int i=0;i<n;i++){
        int x1,x2,y1,y2;
        cin>>x1>>y1>>x2>>y2;
        y[i]=y1;
        y[i+n]=y2;
        evt[i]={x1,y1,y2,1};
        evt[i+n]={x2,y1,y2,-1};
    }
    sort(y.begin(),y.end());
    y.erase(unique(y.begin(),y.end()),y.end());
    sort(evt.begin(),evt.end(),[](const Event&a,const Event&b){return a.x<b.x;});
    SegTree seg(y);
    int ans=0;
    int lastx=evt[0].x;
    int m=evt.size();
    for(int i=0;i<m;){
        int x=evt[i].x;
        ans+=(seg.query()*(x-lastx));
        while(i<m&&evt[i].x==x){
            int l=lower_bound(y.begin(),y.end(),evt[i].y1)-y.begin();
            int r=lower_bound(y.begin(),y.end(),evt[i].y2)-y.begin()-1;
            seg.updateR(l,r,evt[i].k);
            i++;
        }
        lastx=x;
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