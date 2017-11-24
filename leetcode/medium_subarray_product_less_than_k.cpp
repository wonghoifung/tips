#include <vector>
#include <iostream>
#include <stdio.h>

using namespace std;

class Solution {
public:
    int numSubarrayProductLessThanK(vector<int>& nums, int k) {
		int p=0;
		int product=1;
		int sum=0;
		for (int i=0; i<nums.size(); ++i) {
			if (p<i) p=i;
			// expand the window as large as possible
			while (p<nums.size() && nums[p]*product<k) {
				product *= nums[p];
				p+=1;
			}
			// compute the number of valid subarray
			sum += p-i;
			if (p!=i) product/=nums[i];
		}
		return sum;
    }
};

int main() {
	vector<int> nums = {10,5,2,6};
	int k = 100; // expected 8
	cout << Solution().numSubarrayProductLessThanK(nums,k) << endl;
}

