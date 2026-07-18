#include<bits/stdc++.h>
#define int long long int
#define db long double
#define E 1e-8
#define Tcase 0

using namespace std;

//模板原题: 洛谷P2455 Link: https://www.luogu.com.cn/problem/P2455
struct GaussianElimination {
    int n;
    vector<vector<db>>a;//n行n+1列的增广矩阵
    vector<db>answer;
    GaussianElimination(int size){//根据未知数个数建立空的增广矩阵
        n=size;
        a.assign(n,vector<db>(n+1,0));
        answer.assign(n,0);
    }

    //高斯-约旦消元: 返回1表示唯一解,0表示无穷多解,-1表示无解
    int solve(){
        int row=0;
        vector<int>where(n,-1);
        for(int col=0;col<n&&row<n;col++){
            int pivot=row;
            for(int i=row+1;i<n;i++){
                if(fabs(a[i][col])>fabs(a[pivot][col])) pivot=i;
            }
            if(fabs(a[pivot][col])<E) continue;
            swap(a[pivot],a[row]);
            where[col]=row;
            //将主元所在行归一化,使主元变成1
            db divisor=a[row][col];
            for(int j=col;j<=n;j++) a[row][j]/=divisor;
            //消去当前列其余各行.使这一列只剩下主元1
            for(int i=0;i<n;i++){
                if(i==row||fabs(a[i][col])<E) continue;
                db multiple=a[i][col];
                for(int j=col;j<=n;j++) a[i][j]-=multiple*a[row][j];
            }
            row++;
        }
        //若出现 0*x1+...+0*xn=非零数,则方程组无解
        for(int i=0;i<n;i++){
            bool all_zero=true;
            for(int j=0;j<n;j++){
                if(fabs(a[i][j])>=E){
                    all_zero=false;
                    break;
                }
            }
            if(all_zero&&fabs(a[i][n])>=E) return -1;
        }
        //没有矛盾方程但主元个数少于未知数个数,说明存在自由元
        if(row<n) return 0;
        //消元已经把每个主元列化为单位列,可直接读出唯一解
        for(int col=0;col<n;col++){
            answer[col]=a[where[col]][n];
            if(fabs(answer[col])<E) answer[col]=0;
        }
        return 1;
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    int n;
    cin>>n;
    GaussianElimination gauss(n);
    for(int i=0;i<n;i++){
        for(int j=0;j<=n;j++) cin>>gauss.a[i][j];
    }
    int status=gauss.solve();
    if(status==-1){
        cout<<-1<<endl;
    } 
    else if(status==0){
        cout<<0<<endl;
    } 
    else{
        for(int i=0;i<n;i++){
            cout<<"x"<<i+1<<"="<<fixed<<setprecision(2)<<gauss.answer[i]<<endl;
        }
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