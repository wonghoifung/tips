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
	int maxProduct(vector<int>& nums) {
		if (nums.empty()) return 0;
		int minv = nums.back();
		int maxv = nums.back();
		int res = maxv;
		for (int i = nums.size() - 2; i >= 0; --i)
		{
			if (nums[i] > 0)
			{
				maxv = max(nums[i], nums[i] * maxv);
				minv = min(nums[i], nums[i] * minv);
			}
			else
			{
				int tmp = maxv;
				maxv = max(nums[i], nums[i] * minv);
				minv = min(nums[i], nums[i] * tmp);
			}
			res = max(res, maxv);
		}
		return res;
	}
};

int main()
{
	Solution s;
	{
		vector<int> nums{ 0, 2 };
		cout << s.maxProduct(nums) << endl;
	}

	std::cin.get();
	return 0;
}




