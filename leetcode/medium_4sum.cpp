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
	vector<vector<int>> fourSum(vector<int>& nums, int target) {
		vector<vector<int>> res;
		if (nums.size() < 4) return res;
		sort(nums.begin(), nums.end());
		int mx = nums.back();
		if (4 * nums[0] > target || 4 * mx < target) return res;
		int i, z;
		int len = nums.size();
		for (i = 0; i < len; ++i) {
			z = nums[i];
			if (i > 0 && z == nums[i - 1]) continue;
			if (z + 3 * mx < target) continue;
			if (4 * z > target) break;
			if (4 * z == target) {
				if (i + 3 < len && nums[i + 3] == z) res.push_back({ z, z, z, z });
				break;
			}
			threeSum(nums, target - z, i + 1, len - 1, res, z);
		}
		return res;
	}
	void threeSum(vector<int>& nums, int target, int low, int high, vector<vector<int>>& foursumvec, int z1) {
		if (low + 1 >= high) return;
		int mx = nums[high];
		if (3 * nums[low] > target || 3 * mx < target) return;
		int i, z;
		for (i = low; i < high - 1; ++i) {
			z = nums[i];
			if (i > low && z == nums[i - 1]) continue;
			if (z + 2 * mx < target) continue;
			if (3 * z > target) break;
			if (3 * z == target) {
				if (i + 1 < high && nums[i + 2] == z) foursumvec.push_back({ z1, z, z, z });
				break;
			}
			twoSum(nums, target - z, i + 1, high, foursumvec, z1, z);
		}
	}
	void twoSum(vector<int>& nums, int target, int low, int high, vector<vector<int>>& foursumvec, int z1, int z2) {
		if (low >= high) return;
		if (2 * nums[low] > target || 2 * nums[high] < target) return;
		int i = low, j = high, sum, x;
		while (i < j)
		{
			sum = nums[i] + nums[j];
			if (sum == target) {
				foursumvec.push_back({ z1, z2, nums[i], nums[j] });
				x = nums[i];
				while (++i < j && x == nums[i]);
				x = nums[j];
				while (i < --j && x == nums[j]);
			}
			if (sum < target) i += 1;
			if (sum > target) j -= 1;
		}
	}
};

int main()
{
	Solution s;
	{
		vector<int> nums{ 2, 1, 0, -1 };
		auto res = s.fourSum(nums, 2);
		for (const auto& v : res) {
			copy(v.begin(), v.end(), ostream_iterator<int>(cout, " "));
			cout << endl;
		}
	}

	std::cin.get();
	return 0;
}




