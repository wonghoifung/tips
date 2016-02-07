#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm> 
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <set>

using namespace std;

class Solution {
public:
    string itos(int i) {
        stringstream ss;
        ss << i;
        string res;
        ss >> res;
        return res;
    }
    string largestNumber(vector<int>& nums) {
        if (nums.size()==0) {
            return "";
        }
        vector<string> snums(nums.size());
        for (int i=0; i<nums.size(); ++i) {
            snums[i] = itos(nums[i]);
        }
        sort(snums.begin(), snums.end(), [](const string& l, const string& r){
            string s1 = l + r;
            string s2 = r + l;
            return s1 < s2;
        });
        if (snums[snums.size()-1].at(0)=='0') {
            return "0";
        }
        stringstream ss;
        for (int i=(int)snums.size()-1; i>=0; --i) {
            ss << snums[i];
        }
        return ss.str();
    }
};

int main(int argc, char** argv)
{
    Solution s;
    {
        vector<int> nums{3, 30, 34, 5, 9};
        cout << s.largestNumber(nums) << endl;
    }
    {
        vector<int> nums{1,1,1};
        cout << s.largestNumber(nums) << endl;
    }
    {
        vector<int> nums{824,938,1399,5607,6973,5703,9609,4398,8247};
        cout << s.largestNumber(nums) << endl;
    }
    return 0;
}

