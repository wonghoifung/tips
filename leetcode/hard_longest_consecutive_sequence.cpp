#include <iostream>
#include <vector>
#include <algorithm> 
#include <unordered_map>
#include <unordered_set>
#include <stack>

using namespace std;

class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        if (nums.empty()) {
            return 0;
        }
        unordered_set<int> s;
        for (int i=0; i<nums.size(); ++i) {
            s.insert(nums[i]);
        }
        int maxlen=1;
        for (int num:nums) {
            if (s.count(num)) {
                s.erase(num);
                int val = num;
                int sum = 1;
                while (s.count(val-1)) {
                    s.erase(val-1);
                    val-=1;
                }
                sum += num - val;
                val = num;
                while (s.count(val+1)) {
                    s.erase(val+1);
                    val+=1;
                }
                sum+=val-num;
                maxlen=max(maxlen, sum);
            }
        }
        return maxlen;
    }
    int longestConsecutiveSort(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int maxlen=1;
        int curlen=1;
        for (int i=1; i<nums.size(); ++i) {
            if (nums[i]==nums[i-1]) {
                
            } else if (nums[i]-nums[i-1]==1) {
                curlen+=1;
            } else {
                maxlen=max(maxlen,curlen);
                curlen=1;
            }
            if (i==nums.size()-1) {
                maxlen=max(maxlen, curlen);
            }
        }
        return maxlen;
    }
};

int main(int argc, char** argv)
{
    Solution s;
    {
        vector<int> nums{100, 4, 200, 1, 3, 2};
        cout << s.longestConsecutive(nums) << endl;
    }
    
    return 0;
}

