#include <iostream>
#include <sstream>
#include <string>
#include <ctype.h>
using namespace std;
class Solution {
    void parse(const string& c, int& real, int& imaginary) {
        bool realneg=false;
        bool realstart=false;
        int i=0;
        char cc=c.at(i);
        while (cc!='+') {
            if (!realstart && cc=='-') realneg=true;
            else if (!realstart && isdigit(cc)) {
                realstart=true;
                real=cc-'0';
            }
            else if (realstart && isdigit(cc)) {
                real *= 10;
                real += (cc-'0');
            }
            i+=1;
            cc=c.at(i);
        }
        if (realneg) real *= -1;
        i+=1; // skip '+'
        bool imagneg=false;
        bool imagstart=false;
        cc=c.at(i);
        while (cc!='i') {
            if (!imagstart && cc=='-') imagneg=true;
            else if (!imagstart && isdigit(cc)) {
                imagstart=true;
                imaginary=cc-'0';
            }
            else if (imagstart && isdigit(cc)) {
                imaginary *= 10;
                imaginary += (cc-'0');
            }
            i+=1;
            cc=c.at(i);
        }
        if (imagneg) imaginary *= -1;
    }
public:
    string complexNumberMultiply(string a, string b) {
        int areal,aimag,breal,bimag;
        parse(a,areal,aimag);
        parse(b,breal,bimag);
        int real = areal * breal + aimag * bimag * -1;
        int imag = areal * bimag + breal * aimag;
        stringstream ss;
        ss << real << '+' << imag << 'i';
        return ss.str();
    }
};
int main() {
    cout << Solution().complexNumberMultiply("1+1i","1+1i") << endl;//0+2i
    cout << Solution().complexNumberMultiply("1+-1i","1+-1i") << endl;//0+-2i
}
