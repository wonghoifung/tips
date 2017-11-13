#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
	int pivotIndex(vector<int>& nums) {
		if (nums.empty()) return -1;
		if (nums.size()==1) return 0;
		vector<int> leftSums(nums.size(),0);
		vector<int> rightSums(nums.size(),0);
		for (int i=0; i<nums.size(); ++i) {
			leftSums[i] += nums[i];
			if (i>0) leftSums[i] += leftSums[i-1];
			rightSums[nums.size()-1-i] += nums[nums.size()-1-i];
			if (i>0) rightSums[nums.size()-1-i] += rightSums[nums.size()-i]; 
		}
		if (rightSums[1]==0) return 0;
		for (int i=1; i<nums.size()-1; ++i) {
			if (leftSums[i-1]==rightSums[i+1]) return i;
		}
		if (leftSums[nums.size()-2]==0) return nums.size() - 1;
		return -1;
	}
};

int main() {
	vector<int> nums = {-1,-1,-1,1,1,1};
	cout << Solution().pivotIndex(nums) << endl;
}

