#include <iostream>
#include <vector>
#include <algorithm> 
#include <unordered_map>
#include <unordered_set>
#include <stack>

using namespace std;

class Solution {
public:
    int findKthLargestHelper(vector<int>& nums, int k, int l, int r) {
        int mid = l;
        for (int i=mid; i<r; ++i) {
            if (nums[i]<=nums[r]) swap(nums[mid++], nums[i]);
        }
        swap(nums[mid], nums[r]);
        if (mid==k) return nums[mid];
        else if (mid>k) return findKthLargestHelper(nums, k, l, mid-1);
        else return findKthLargestHelper(nums, k, mid+1, r);
    }
    int findKthLargest(vector<int>& nums, int k) {
        return findKthLargestHelper(nums, nums.size()-k, 0, nums.size()-1);
    }
};

int main(int argc, char** argv)
{
    Solution s;
    (void)s;
    
    return 0;
}

