#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <stdint.h>

using namespace std;

/* https://discuss.leetcode.com/topic/76243/java-15-ms-c-3-ms-o-ns-iterative-dp-solution-using-subset-sum-with-explanation */
class Solution {
	int subsetSum(vector<int>& nums, int s) {
		vector<int> dp(s + 1, 0);
		dp[0] = 1;
		for (int n : nums) {
			for (int i = s; i >= n; --i) {
				dp[i] += dp[i - n];
			}
		}
		return dp[s];
	}
public:
	int findTargetSumWays(vector<int>& nums, int S) {
		int sum = accumulate(nums.begin(), nums.end(), 0);
		return sum < S || (S + sum) & 1 ? 0 : subsetSum(nums, (S + sum) >> 1);
	}
};

int main() {
	{
		vector<int> nums{ 1, 1, 1, 1, 1 };
		cout << Solution().findTargetSumWays(nums, 3) << endl; // 5
	}

	cin.get();
}

