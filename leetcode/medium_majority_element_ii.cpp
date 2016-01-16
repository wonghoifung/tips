#include <iostream>
#include <vector>
#include <algorithm> 
#include <unordered_map>
#include <unordered_set>
#include <stack>

using namespace std;

class Solution {
public:
    vector<int> majorityElement(vector<int>& nums) {
        vector<int> res;
        if(nums.empty()) return res;
        int count[2]{0};
        int x[2]{0};
        x[0]=0;x[1]=1;
        for (int i=0; i<nums.size(); ++i) {
            if (x[0]==nums[i]) {
                count[0]+=1;
            } else if(x[1]==nums[i]) {
                count[1]+=1;
            } else if(count[0]==0) {
                x[0]=nums[i];
                count[0]=1;
            } else if(count[1]==0) {
                x[1]=nums[i];
                count[1]=1;
            } else {
                count[0]-=1;
                count[1]-=1;
            }
        }
        fill(begin(count), end(count), 0);
        for (int i:nums) {
            if(i==x[0]) count[0]+=1;
            else if(i==x[1]) count[1]+=1;
        }
        int n = (int)nums.size();
        int cnt = n / 3;
        if (count[0]>cnt) {
            res.push_back(x[0]);
        }
        if (count[1]>cnt && find(res.begin(), res.end(), x[1])==res.end()) {
            res.push_back(x[1]);
        }
        return res;
    }
    vector<int> majorityElement1(vector<int>& nums) {
        vector<int> res;
        int n = (int)nums.size();
        int cnt = n / 3;
        sort(nums.begin(), nums.end());
        int current = 1;
        for (int i=1; i<n; ++i) {
            if (nums[i]==nums[i-1]) {
                current += 1;
            } else {
                if (current>cnt) {
                    res.push_back(nums[i-1]);
                }
                current = 1;
            }
        }
        if (nums.size() && current>cnt) {
            res.push_back(nums.back());
        }
        return res;
    }
};

int main(int argc, char** argv)
{
    Solution s;
    vector<int> nums;
    auto res = s.majorityElement(nums);
    
    return 0;
}

