#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Solution {
public:
    int singleNonDuplicate(vector<int>& nums) {
    	if (nums.empty() || nums.size() % 2 == 0) return 0;
		if (nums.size() == 1) return nums[0];    
		int beg = 0; 
		int end = nums.size() - 1;
		int mid = nums.size() / 2;
		while (mid > 0) {
			//cout << "beg:" << beg << ", end:" << end << ", mid:" << mid << endl;
			if (beg == end) return nums[beg];
			if (nums[mid] == nums[mid+1]) mid -= 1;
			if ((mid-beg+1) % 2 == 0) {beg = mid + 1;}
			else {end = mid;} 
			mid = (end-beg+1) / 2 + beg;
		}
		return nums[0];
    }
};

int main()
{
	Solution s;
	{
	vector<int> nums = {1,1,2,3,3,4,4,8,8};
	cout << s.singleNonDuplicate(nums) << endl;
	}
	{
	vector<int> nums = {3,3,7,7,10,11,11};
	cout << s.singleNonDuplicate(nums) << endl;
	}
}

