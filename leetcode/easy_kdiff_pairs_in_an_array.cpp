#include <iostream>
#include <vector>
#include <set>
using namespace std;

class Solution {
public:
    int findPairs(vector<int>& nums, int k) {
        set<int> s;
        for (int i=0; i<nums.size(); ++i) {
            for (int j=i+1; j<nums.size(); ++j) {
                int smaller = min(nums[i],nums[j]);
                int bigger = max(nums[i],nums[j]);
                if (bigger-smaller==k) {
                    s.insert(smaller);
                }
            }
        }
        return s.size();
    }
};

int main() {
    {
        vector<int> nums{1,3,1,5,4};
        cout << Solution().findPairs(nums,0) << endl;
    } 
}
