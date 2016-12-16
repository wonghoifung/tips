#include <vector>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <sstream>
#include <algorithm>
#include <queue>
#include <set>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

class Solution {
public:
#if 0
	bool canPartition(const vector<int>& nums) {
		if (nums.empty()) return true;
		int volumn = 0;
		for (int num : nums) volumn += num;
		if (volumn % 2) return false;
		volumn /= 2;
		vector<bool> dp(volumn + 1, false);
		dp[0] = true; // dp[i] means can i be summed up to
		for (int i = 1; i <= nums.size(); ++i) { 
			for (int j = volumn; j >= nums[i - 1]; --j) { // from volumn to every num in nums
				dp[j] = dp[j] || dp[j - nums[i - 1]];
			}
		}
		return dp[volumn];
	}
#else
	bool canPartition(const vector<int>& nums) {
		int sum = 0;
		for (int num : nums) sum += num;
		if (sum & 1) return false;
		sum /= 2;
		int n = nums.size();
		vector<vector<bool>> dp(n + 1, vector<bool>(sum + 1, false));
		dp[0][0] = true; // dp[i][j] means whether the specific sum j can be gotten from the first i numbers
		for (int i = 1; i < n + 1; ++i) dp[i][0] = true;
		for (int j = 1; j < sum + 1; ++j) dp[0][j] = false;
		for (int i = 1; i < n + 1; ++i) {
			for (int j = 1; j < sum + 1; ++j) {
				dp[i][j] = dp[i - 1][j]; // do not pick nums[i-1] to sum up to j
				if (j >= nums[i - 1]) { // to pick nums[i-1]
					dp[i][j] = (dp[i][j] || dp[i - 1][j - nums[i - 1]]);
				}
			}
		}
		return dp[n][sum];
	}
#endif
};

int main() {
	cout << Solution().canPartition({ 1, 5, 11, 5 }) << endl;
	cout << Solution().canPartition({ 1, 2, 3, 5 }) << endl;

	cin.get();
	return EXIT_SUCCESS;
}

