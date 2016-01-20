#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <array>
#include <memory>
#include <sstream>
#include <functional>
#include <algorithm>
#include <bitset>
#include <tuple>
#include <numeric>
#include <initializer_list>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <stdint.h>

using namespace std;

class Solution {
public:
	int findMin(vector<int>& nums) {
		if (nums.empty()) return 0;
		if (nums.size() == 1) return nums.back();
		if (nums.size() == 2) return nums[0] < nums[1] ? nums[0] : nums[1];
		int l = 0;
		int r = nums.size() - 1;
		while (r - l > 1)
		{
			int m = (r - l) / 2 + l;
			if (m == l || m == r) break;
			if (nums[m] < nums[m - 1]) return nums[m];
			if (nums[m] == nums[r] && nums[m] == nums[l]) {
				r = r - 1;
				l = l + 1;
				
			}
			else if (nums[m] <= nums[r]) {
				r = m - 1;
			}
			else if (nums[m] >= nums[l]) {
				l = m + 1;
			}
		}
		int res = INT_MAX;
		for (int i = l; i <= r; ++i)
		{
			res = min(res, nums[i]);
		}
		return res;
	}
};

int main()
{
	Solution s;
	{
		vector<int> nums{ 3, 3, 1, 3 };
		cout << s.findMin(nums) << endl;
	}

	std::cin.get();
	return 0;
}




