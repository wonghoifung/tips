#include <iostream>
#include <sstream>
#include <vector>
#include <deque>
#include <algorithm> 
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <set>
#include <map>

using namespace std;

class Solution {
public:
    string toString(long num) {
        stringstream ss;
        ss << num;
        string res;
        ss >> res;
        return res;
    }
    bool isAdditive(string str, long num1, long num2) {
        if (str.empty()) return true;
        long sum = num1 + num2;
        string s = toString(sum);
        if (str.substr(0,s.size())!=s) return false;
        return isAdditive(str.substr(s.size()), num2, sum);
    }
    bool isAdditiveNumber(string num) {
        int numlen = num.size();
        for (int i=1; i<=(numlen-1)/2; ++i) {
            if (num[0]=='0' && i>=2) break;
            for (int j=i+1; numlen-j>=j-i && numlen-j>=i; ++j) {
                if (num[i]=='0' && j-i>=2) break;
                long num1 = stol(num.substr(0,i));
                long num2 = stol(num.substr(i,j-i));
                string rem = num.substr(j);
                if (isAdditive(rem, num1, num2)) return true;
            }
        }
        return false;
    }
    
};

int main(int argc, char** argv)
{
    Solution s;
    {
    }
    return 0;
}

