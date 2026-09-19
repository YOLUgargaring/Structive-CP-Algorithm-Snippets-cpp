#include<bits/stdc++.h>
#define int long long int
#define coutfix(x) fixed<<setprecision(x)
#define TCASE 0

using namespace std;

//模板原题: 洛谷P2742 Link: https://www.luogu.com.cn/problem/P2742
struct Point2D{
    double x;
    double y;

    bool operator<(const Point2D &o) const{
        if(x!=o.x) return x<o.x;
        return y<o.y;
    }

    bool operator==(const Point2D &o) const{
        return x==o.x&&y==o.y;
    }
};

class Convex2D{//Andrew算法求二维凸包
private:
    int n;
    vector<Point2D>point;
    vector<Point2D>convex;
    double convex_C=-1;
    double convex_S=-1;

public:
    Convex2D(const vector<Point2D> &p){
        n=p.size();
        point=p;
    }

    double cross(Point2D P,Point2D Q){//计算对于原点O,OP×OQ
        return P.x*Q.y-P.y*Q.x;
    }

    double cross(Point2D A,Point2D B,Point2D C){//计算叉积AB×AC
        return (B.x-A.x)*(C.y-A.y)-(B.y-A.y)*(C.x-A.x);
    }

    bool turnLeft(Point2D A,Point2D B,Point2D C){//判断A->B->C是否左转
        return cross(A,B,C)>0;
    }

    double dist(Point2D A,Point2D B){//计算A,B两点距离|AB|
        return hypot(A.x-B.x,A.y-B.y);
    }

    void build(){
        sort(point.begin(),point.end());
        point.erase(unique(point.begin(),point.end()),point.end());
        n=point.size();
        if(n<=1){
            convex=point;
            return;
        }
        for(auto P:point){//构造下凸壳
            //非左转弹出
            while(convex.size()>=2&&!turnLeft(convex[convex.size()-2],convex.back(),P)) convex.pop_back();
            convex.push_back(P);
        }
        int t=convex.size();//下凸壳的点数
        for(int i=n-2;i>=0;i--){
            while(convex.size()>=t+1&&!turnLeft(convex[convex.size()-2],convex.back(),point[i])) convex.pop_back();
            convex.push_back(point[i]);
        }
        convex.pop_back();//起点重复计算除去
    }

    double calc_C(){//计算凸包周长
        if(convex_C!=-1) return convex_C;
        double res=0;
        int m=convex.size();
        // C=∑(i:0->m-1){|P[i]P[(i+1) mod m]|}
        for(int i=0;i<m;i++) res+=dist(convex[i],convex[(i+1)%m]);
        convex_C=res;
        return res;
    }

    double calc_S(){//计算凸包围成的面积
        if(convex_S!=-1) return convex_S;
        double res=0;
        int m=convex.size();
        // S=1/2|∑(i:0->m-1){P[i]×P[(i+1) mod m]}|
        for(int i=0;i<m;i++) res+=cross(convex[i],convex[(i+1)%m]);
        res=fabs(res)/2.0;
        convex_C=res;
        return res;
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n;
    cin>>n;
    vector<Point2D>p(n);
    for(Point2D &t:p) cin>>t.x>>t.y;
    Convex2D cvx(p);
    cvx.build();
    double ans=cvx.calc_C();
    cout<<coutfix(2)<<ans;
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