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
	int calculateMinimumHP(vector<vector<int>>& dungeon) {
		if (dungeon.empty()) return 0;
		int m = dungeon.size();
		int n = dungeon[0].size();
		vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));;
		for (int i = 0; i <= m; ++i) dp[i][n] = INT_MAX;
		for (int i = 0; i <= n; ++i) dp[m][i] = INT_MAX;
		dp[m - 1][n] = 1;
		for (int r = m - 1; r >= 0; --r)
		{
			for (int c = n - 1; c >= 0; --c)
			{
				int target = min(dp[r + 1][c], dp[r][c + 1]);
				if (dungeon[r][c] < target) dp[r][c] = target - dungeon[r][c];
				else dp[r][c] = 1;
			}
		}
		return dp[0][0];
	}
};

int main()
{
	Solution s;
	{
		vector<vector<int>> dungeon{ { -2, -3, 3 }, { -5, -10, 1 }, { 10, 30, -5 } }; // 7 r r d d
		cout << s.calculateMinimumHP(dungeon) << endl;
	}

	std::cin.get();
	return 0;
}




