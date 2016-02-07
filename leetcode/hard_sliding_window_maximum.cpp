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
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        vector<int> res;
        if (k == 0 || nums.empty()) {
            return res;
        }
        deque<int> window;
        map<int, int> counts;
        int curmax = INT_MIN;
        for (int i=0; i<k; ++i) {
            window.push_back(nums[i]);
            curmax = max(curmax, nums[i]);
            counts[nums[i]] += 1;
        }
        res.push_back(curmax);
        for (int i=k; i<nums.size(); ++i) {
            int redundant = window.front();
            auto it = counts.find(redundant);
            if (it->second == 1) {
                counts.erase(it);
                if (curmax == redundant) {
                    if (counts.empty()) {
                        curmax = INT_MIN;
                    } else {
                        curmax = counts.rbegin()->first;
                    }
                }
            } else {
                it->second -= 1;
            }
            window.pop_front();
            window.push_back(nums[i]);
            curmax = max(curmax, nums[i]);
            counts[nums[i]] += 1;
            res.push_back(curmax);
        }
        return res;
    }
    
};

int main(int argc, char** argv)
{
    Solution s;
    {
        vector<int> nums{1,3,-1,-3,5,3,6,7};
        auto res = s.maxSlidingWindow(nums, 3);
        copy(res.begin(), res.end(), ostream_iterator<int>(cout, " "));
        cout << endl;
    }
    {
        vector<int> nums{1,-1};
        auto res = s.maxSlidingWindow(nums, 1);
        copy(res.begin(), res.end(), ostream_iterator<int>(cout, " "));
        cout << endl;
    }
    return 0;
}

