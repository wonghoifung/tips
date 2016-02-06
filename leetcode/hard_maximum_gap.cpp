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
    int maximumGap(vector<int>& nums) {
        if (nums.size() < 2) return 0;
        int minn = nums[0];
        int maxn = nums[0];
        for (int i:nums) {
            minn = min(minn,i);
            maxn = max(maxn,i);
        }
        int gap = (int)ceil((double)(maxn-minn)/(nums.size()-1));
        vector<int> bucketsmin(nums.size()-1,INT_MAX);
        vector<int> bucketsmax(nums.size()-1,INT_MIN);
        for (int i:nums) {
            if (i==minn || i==maxn) continue;
            int idx = (i - minn) / gap;
            bucketsmin[idx] = min(i, bucketsmin[idx]);
            bucketsmax[idx] = max(i, bucketsmax[idx]);
        }
        int maxgap = 0;
        int previous = minn;
        for (int i=0; i<nums.size()-1; ++i) {
            if (bucketsmin[i]==INT_MAX && bucketsmax[i]==INT_MIN) {
                continue; // empty bucket
            }
            maxgap = max(maxgap, bucketsmin[i]-previous);
            previous = bucketsmax[i];
        }
        maxgap = max(maxgap, maxn-previous);
        return maxgap;
    }
    
};

int main(int argc, char** argv)
{
    Solution s;
    {
        
    }
    
    return 0;
}

