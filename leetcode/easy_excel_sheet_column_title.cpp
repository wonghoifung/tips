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
    string convertToTitle(int n) {
        string res;
        while (n != 0) {
            char c = (n-1)%26+'A';
            n = (n-1)/26;
            res=c+res;
        }
        return res;
    }
    
};

int main(int argc, char** argv)
{
    Solution s;
    {
        cout << s.convertToTitle(100000) << endl;
    }
    
    return 0;
}

