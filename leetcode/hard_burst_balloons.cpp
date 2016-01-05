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
	int burst(vector<vector<int>>& memo, vector<int>& nums, int left, int right) {
		if (left + 1 == right) return 0;
		if (memo[left][right] > 0) return memo[left][right];
		int res = 0;
		for (int i = left + 1; i < right; ++i)
		{
			res = max(res, nums[left] * nums[i] * nums[right] + burst(memo, nums, left, i) + burst(memo, nums, i, right));
		}
		memo[left][right] = res;
		return res;
	}
	int maxCoins1(vector<int>& nums) {
		if (nums.empty()) return 0;
		vector<int> opnums(nums.size() * 2);
		int n = 1;
		for (auto i : nums) if (i != 0) opnums[n++] = i;
		opnums[0] = opnums[n++] = 1;

		vector<vector<int>> memo(n, vector<int>(n, 0));
		return burst(memo, opnums, 0, n - 1);
	}

	int value(vector<int>& nums, int i) {
		if (i < 0 || i >= nums.size()) return 1;
		return nums[i];
	}
	int maxCoins(vector<int>& nums) {
		if (nums.empty()) return 0;
		vector<vector<int>> dp(nums.size(), vector<int>(nums.size(), 0));
		for (int len = 1; len <= nums.size(); ++len)
		{
			for (int start = 0; start <= nums.size() - len; ++start)
			{
				int end = start + len - 1;
				for (int i = start; i <= end; ++i)
				{
					int coins = nums[i] * value(nums, start - 1) * value(nums, end + 1);
					coins += (i != start ? dp[start][i - 1] : 0);
					coins += (i != end ? dp[i + 1][end] : 0);
					dp[start][end] = max(dp[start][end], coins);
				}
			}
		}
		return dp[0][nums.size() - 1];
	}
};

int main()
{
	Solution solu;
	{
		vector<int> nums{ 3, 1, 5, 8 };
		cout << solu.maxCoins(nums) << endl; // 167
	}
	{
		vector<int> nums{ 35, 16, 83, 87, 84, 59, 48, 41, 20, 54 };
		cout << solu.maxCoins(nums) << endl;
	}
	std::cin.get();
	return 0;
}




