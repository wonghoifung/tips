#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm> 
#include <unordered_map>
#include <unordered_set>
#include <stack>

using namespace std;

class Solution {
public:
    string fractionToDecimal(int numerator, int denominator) {
        bool isNegative=((numerator < 0 && denominator > 0) || (numerator > 0 && denominator < 0)) ? true : false;
        long numeratorL=abs((long)numerator);
        long denominatorL=abs((long)denominator);
        unordered_map<long, int> prevRemains;
        stringstream ss;
        long quotient = numeratorL/denominatorL;
        ss<<quotient;
        numeratorL %= denominatorL;
        
        int dotIndex = (int)ss.str().size();
        if(numeratorL != 0) ss << ".";
        
        int quotientIndex = 0;
        while (numeratorL != 0) {
            numeratorL *= 10;
            quotient = abs(numeratorL/denominatorL);
            if (!prevRemains.count(numeratorL)) {
                ss<<quotient;
                prevRemains[numeratorL] = quotientIndex++;
            } else {
                int firstIndex = 1 + prevRemains[numeratorL] + dotIndex;
                string tmp=ss.str();
                string res = tmp.substr(0,firstIndex) + "(" + tmp.substr(firstIndex) + ")";
                if (isNegative) {
                    return "-" + res;
                } else {
                    return res;
                }
            }
            numeratorL %= denominatorL;
        }
        
        if (isNegative) return "-" + ss.str();
        return ss.str();
    }
};

int main(int argc, char** argv)
{
    Solution s;
    {
        
    }
    
    return 0;
}

