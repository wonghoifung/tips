#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>s
#include <algorithm>
#include <numeric>
#include <stdint.h>

using namespace std;

/* https://discuss.leetcode.com/topic/76327/c-dp-solution-with-explanation/2 */
class Solution {
public:
	bool PredictTheWinner(vector<int>& nums) {
		int n = nums.size();
		vector<vector<int>> dp(n, vector<int>(n));
		for (int i = 0; i < n; ++i) dp[i][i] = nums[i];
		for (int i = 1; i < n; ++i) {
			for (int j = 0; j + i < n; ++j) {
				dp[j][j + i] = max(nums[j + i] - dp[j][j + i - 1], nums[j] - dp[j + 1][j + i]);
			}
		}
		return dp[0][n - 1] >= 0;
	}
};

int main() {
	{
		vector<int> nums{ 1, 5, 2 };
		cout << boolalpha << Solution().PredictTheWinner(nums) << endl; // false
	}
	{
		vector<int> nums{ 1, 5, 233, 7 };
		cout << boolalpha << Solution().PredictTheWinner(nums) << endl; // true
	}

	cin.get();
}

