#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class Solution {
public:
    int findUnsortedSubarray(vector<int>& nums) {
        if (nums.size() == 0) return 0;
        int leftMax = nums[0];
        int rightMostDisorder = 0;
        for (int i=1; i<nums.size(); ++i) {
            if (nums[i] < leftMax) {
                rightMostDisorder = i;
            } else {
                leftMax = nums[i];
            }
        }
        int rightMin = nums[nums.size()-1];
        int leftMostDisorder = nums.size()-1;
        for (int i=nums.size()-2; i>=0; --i) {
            if (nums[i] > rightMin) {
                leftMostDisorder = i;
            } else {
                rightMin = nums[i];
            }
        }
        if (leftMostDisorder < rightMostDisorder) {
            return rightMostDisorder - leftMostDisorder + 1;
        }
        return 0;
    }
};

int main() {

}
