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
	vector<vector<int>> threeSum(vector<int>& nums) {
		vector<vector<int>> res;
		if (nums.size() < 3) return res;
		sort(nums.begin(), nums.end());
		int len = nums.size();
		for (int i = 0; i < len; ++i)
		{
			if (nums[i] > 0) break;
			if (i > 0 && nums[i] == nums[i - 1]) continue;
			int target = 0 - nums[i];
			int j = i + 1, k = len - 1;
			while (j < k)
			{
				if (nums[j] + nums[k] == target) {
					res.push_back({ nums[i], nums[j], nums[k] });
					while (j < k && nums[j] == nums[j + 1]) j += 1;
					while (j < k && nums[k] == nums[k - 1]) k -= 1;
					j += 1; k -= 1;
				}
				else if (nums[j] + nums[k] < target) j += 1;
				else k -= 1;
			}
		}
		return res;
	}
};

int main()
{
	Solution s;
	{
		vector<int> nums{ 7, -1, 14, -12, -8, 7, 2, -15, 8, 8, -8, -14, -4, -5, 7, 9, 11, -4, -15, -6, 1, -14, 4, 3, 10, -5, 2, 1, 6, 11, 2, -2, -5, -7, -6, 2, -15, 11, -6, 8, -4, 2, 1, -1, 4, -6, -15, 1, 5, -15, 10, 14, 9, -8, -6, 4, -6, 11, 12, -15, 7, -1, -9, 9, -1, 0, -4, -1, -12, -2, 14, -9, 7, 0, -3, -4, 1, -2, 12, 14, -10, 0, 5, 14, -1, 14, 3, 8, 10, -8, 8, -5, -2, 6, -11, 12, 13, -7, -12, 8, 6, -13, 14, -2, -5, -11, 1, 3, -6 };
		auto res = s.threeSum(nums);
		for (const auto& v : res) {
			copy(v.begin(), v.end(), ostream_iterator<int>(cout, " "));
			cout << endl;
		}
	}

	std::cin.get();
	return 0;
}




