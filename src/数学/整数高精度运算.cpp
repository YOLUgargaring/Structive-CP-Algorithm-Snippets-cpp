#include<bits/stdc++.h>
#define int long long int
#define Tcase 0

using namespace std;

//模板原题: 洛谷P1932 Link: https://www.luogu.com.cn/problem/P1932
class BigInteger{//高精度整数运算,支持10^4十进制位整数的加,减,乘,整除,取余
private:
    //每个数组元素保存9位十进制数,低位放在数组前面
    //相比逐位保存十进制数,可以明显减少乘法和除法的循环次数
    const static int BASE=1000000000;
    const static int WIDTH=9;

    vector<int>number;
    int sign;//1表示非负数,-1表示负数

    void removeLeadingZero(){//删除最高位多余的0,并保证0的符号始终为正
        while(!number.empty()&&number.back()==0) number.pop_back();
        if(number.empty()) sign=1;
    }

    int compareAbs(const BigInteger& other) const{//比较两个数的绝对值:小于返回-1,等于返回0,大于返回1
        if(number.size()!=other.number.size()){
            return number.size()<other.number.size()?-1:1;
        }
        for(int i=(int)number.size()-1;i>=0;i--) {
            if(number[i]!=other.number[i]){
                return number[i]<other.number[i]?-1:1;
            }
        }
        return 0;
    }

    BigInteger addAbs(const BigInteger& other) const{//只计算绝对值加法,结果一定非负
        BigInteger ret;
        int carry=0;
        int len=max(number.size(),other.number.size());
        for(int i=0;i<len||carry;i++){
            int sum=carry;
            if(i<(int)number.size()) sum+=number[i];
            if(i<(int)other.number.size()) sum+=other.number[i];
            ret.number.push_back(sum%BASE);
            carry=sum/BASE;
        }
        return ret;
    }

    BigInteger subAbs(const BigInteger& other) const{//只计算绝对值减法,调用前需要保证当前数的绝对值不小于other
        BigInteger ret;
        int borrow=0;
        for(int i=0;i<(int)number.size();i++){
            int cur=number[i]-borrow;
            if(i<(int)other.number.size()) cur-=other.number[i];
            if(cur<0){
                cur+=BASE;
                borrow=1;
            } 
            else borrow=0;
            ret.number.push_back(cur);
        }
        ret.removeLeadingZero();
        return ret;
    }

    BigInteger mulInt(int x) const{//高精度数乘一个普通非负整数,供除法估算商的每一位使用。
        if(x==0||isZero()) return BigInteger();
        BigInteger ret;
        ret.sign=sign;
        int carry=0;
        for(int i=0;i<(int)number.size()||carry;i++){
            int cur=carry;
            if(i<(int)number.size()) cur+=number[i]*x;
            ret.number.push_back(cur%BASE);
            carry=cur/BASE;
        }
        ret.removeLeadingZero();
        return ret;
    }

    BigInteger divInt(int x,int& remainder) const{//高精度数除以普通正整数,返回商,并通过remainder得到余数
        BigInteger ret;
        ret.sign=sign;
        ret.number.assign(number.size(),0);
        remainder=0;
        for(int i=(int)number.size()-1;i>=0;i--){
            int cur=number[i]+remainder*BASE;
            ret.number[i]=cur/x;
            remainder=cur%x;
        }
        ret.removeLeadingZero();
        return ret;
    }

    void shiftBase(){//在最低位插入一个0,相当于当前数乘BASE
        number.insert(number.begin(),0);
    }

    BigInteger absValue() const{//返回当前数的绝对值副本,不修改当前对象
        BigInteger ret=*this;
        ret.sign=1;
        return ret;
    }

public:
    BigInteger(){//默认构造出的高精度整数为0
        sign=1;
    }

    BigInteger(const string& s){//根据字符串构造高精度整数,字符串可以带负号
        read(s);
    }

    void read(const string& s){//将十进制字符串读入BASE进制数组
        number.clear();
        sign=1;
        int start=0;
        if(!s.empty()&&s[0]=='-'){
            sign=-1;
            start=1;
        }
        for(int i=s.size();i>start;i-=WIDTH){
            int left=max(start,i-WIDTH);
            int cur=0;
            for(int j=left;j<i;j++) cur=cur*10+s[j]-'0';
            number.push_back(cur);
        }
        removeLeadingZero();
    }

    string toString() const{//将高精度整数转换回十进制字符串,方便直接输出
        if(isZero()) return "0";
        string ret=(sign==-1)?"-":"";
        ret+=to_string(number.back());
        for(int i=(int)number.size()-2;i>=0;i--){
            string cur=to_string(number[i]);
            ret+=string(WIDTH-cur.size(),'0')+cur;
        }
        return ret;
    }

    bool isZero() const{//判断当前高精度整数是否为0
        return number.empty();
    }

    // operator+ 运算符重载,使高精度整数相加也能写成 A+B
    BigInteger operator+(const BigInteger& other) const{
        BigInteger ret;
        if(sign==other.sign) {
            ret=addAbs(other);
            ret.sign=sign;
            return ret;
        }
        int cmp=compareAbs(other);
        if(cmp==0) return ret;
        if(cmp>0){
            ret=subAbs(other);
            ret.sign=sign;
        }
        else{
            ret=other.subAbs(*this);
            ret.sign=other.sign;
        }
        return ret;
    }

    // operator- 运算符重载,使高精度整数相减也能写成 A-B
    BigInteger operator-(const BigInteger& other) const{
        BigInteger opposite=other;
        if(!opposite.isZero()) opposite.sign*=-1;
        return *this+opposite;
    }

    // operator* 运算符重载,使高精度整数相乘也能写成 A*B
    BigInteger operator*(const BigInteger& other) const{
        BigInteger ret;
        if(isZero()||other.isZero()) return ret;
        ret.sign=sign*other.sign;
        ret.number.assign(number.size()+other.number.size(),0);
        for(int i=0;i<(int)number.size();i++){
            int carry=0;
            for(int j=0;j<(int)other.number.size()||carry;j++){
                int cur=ret.number[i+j]+carry;
                if(j<(int)other.number.size()) cur+=number[i]*other.number[j];
                ret.number[i+j]=cur%BASE;
                carry=cur/BASE;
            }
        }
        ret.removeLeadingZero();
        return ret;
    }

    //同时计算商和余数,避免solve()分别做除法,取余时重复整套长除法
    pair<BigInteger,BigInteger> divmod(const BigInteger& other) const{//返回值pair的first是商,second是余数
        //注: 此模板未作除数为0异常处理!
        BigInteger a=absValue();
        BigInteger b=other.absValue();
        BigInteger quotient,remainder;
        //将除数最高位放大到接近BASE,使每一位商的估计至多向下修正少量次数
        int norm=BASE/(b.number.back()+1);
        a=a.mulInt(norm);
        b=b.mulInt(norm);
        quotient.number.assign(a.number.size(),0);
        for(int i=(int)a.number.size()-1;i>=0;i--){
            remainder.shiftBase();
            remainder.number[0]=a.number[i];
            remainder.removeLeadingZero();
            int high1=(remainder.number.size()<=b.number.size())?0:remainder.number[b.number.size()];
            int high2=(remainder.number.size()<b.number.size())?0:remainder.number[b.number.size()-1];
            int digit=(high1*BASE+high2)/b.number.back();
            remainder=remainder-b.mulInt(digit);
            while(remainder.sign==-1){
                remainder=remainder+b;
                digit--;
            }
            quotient.number[i]=digit;
        }
        quotient.sign=sign*other.sign;
        quotient.removeLeadingZero();
        remainder.sign=sign;
        remainder.removeLeadingZero();
        int unused=0;
        remainder=remainder.divInt(norm,unused);
        return {quotient,remainder};
    }

    // operator/ 与 operator% 分别提供单独求商,求余的调用方式
    // operator/ 运算符重载,使高精度整数相除得出整除结果也能写成 A/B
    BigInteger operator/(const BigInteger& other) const{
        return divmod(other).first;
    }

    // operator% 运算符重载,使高精度整数取余也能写成 A%B
    BigInteger operator%(const BigInteger& other) const{
        return divmod(other).second;
    }
};

/*=============...solving...=============*/
void solve_by_YOLU(){
    string a,b;
    cin>>a>>b;
    BigInteger A(a),B(b);
    cout<<(A+B).toString()<<endl;
    cout<<(A-B).toString()<<endl;
    cout<<(A*B).toString()<<endl;
    // cout<<(A/B).toString()<<endl;
    // cout<<(A%B).toString()<<endl;
    pair<BigInteger,BigInteger>result=A.divmod(B);//推荐此写法,避免重复调用
    cout<<result.first.toString()<<endl;
    cout<<result.second.toString()<<endl;
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