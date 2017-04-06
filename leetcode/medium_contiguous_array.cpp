#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
using namespace std;
class Solution {
public:
	int findMaxLength(vector<int>& nums) {
		for (auto i=0; i<nums.size(); ++i) if (nums[i]==0) nums[i]=-1;
		map<int,int> summ;
		summ[0]=-1;
		int sum=0,cmax=0;
		for (auto i=0; i<nums.size(); ++i) {
			sum += nums[i];
			if (summ.find(sum)!=summ.end()) {
				cmax = max(cmax, i-summ[sum]);
			} else {
				summ[sum]=i;
			}
		}
		return cmax;
	}
};
int main() {
	{
		vector<int> nums{0,1};
		cout<<Solution().findMaxLength(nums)<<endl;//2
	}
	{
		vector<int> nums{0,1,0};
		cout<<Solution().findMaxLength(nums)<<endl;//2
	}
}

