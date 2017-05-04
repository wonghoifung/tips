#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;
class Solution {
public:
    string reverseStr(string s, int k) {
        int dk=2*k;
        int len=s.size();
        stringstream ss;
        for (int start=0; start<len; start+=dk) {
            int mid=start+k;
            int end=start+dk;
            if (end>len) end=len;
            if (mid>len) mid=len;
            string subs=s.substr(start, mid-start);
            reverse(subs.begin(),subs.end());
            ss<<subs;
            ss<<s.substr(mid,end-mid);
        }
        return ss.str();
    }
};
int main() {
    cout << Solution().reverseStr("abcdefg",2) << endl; // bacdfeg
}

