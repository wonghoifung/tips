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
	void dfs_bt(vector<vector<int>>& matrix, vector<vector<int>>& bt, int& curres, int& ddres, int x, int y) {
		int trycnt = 0;
		if (x > 0 && 0 == bt[x - 1][y] && matrix[x][y] < matrix[x - 1][y])
		{
			trycnt += 1;
			bt[x - 1][y] = 1;
			curres += 1;
			dfs_bt(matrix, bt, curres, ddres, x - 1, y);
			curres -= 1;
			bt[x - 1][y] = 0;
		}
		if (y < matrix[0].size() - 1 && 0 == bt[x][y + 1] && matrix[x][y] < matrix[x][y + 1])
		{
			trycnt += 1;
			bt[x][y + 1] = 1;
			curres += 1;
			dfs_bt(matrix, bt, curres, ddres, x, y + 1);
			curres -= 1;
			bt[x][y + 1] = 0;
		}
		if (x < matrix.size() - 1 && 0 == bt[x + 1][y] && matrix[x][y] < matrix[x + 1][y])
		{
			trycnt += 1;
			bt[x + 1][y] = 1;
			curres += 1;
			dfs_bt(matrix, bt, curres, ddres, x + 1, y);
			curres -= 1;
			bt[x + 1][y] = 0;
		}
		if (y > 0 && 0 == bt[x][y - 1] && matrix[x][y] < matrix[x][y - 1])
		{
			trycnt += 1;
			bt[x][y - 1] = 1;
			curres += 1;
			dfs_bt(matrix, bt, curres, ddres, x, y - 1);
			curres -= 1;
			bt[x][y - 1] = 0;
		}
		if (trycnt == 0) {
			ddres = max(ddres, curres);
		}
	}
	int dfs_dp(vector<vector<int>>& matrix, vector<vector<int>>& dp, int x, int y) {
		if (dp[x][y] > 0) return dp[x][y];
		int maxd = 0;
		if (x > 0 && matrix[x][y] > matrix[x - 1][y]) {
			int up = dfs_dp(matrix, dp, x - 1, y);
			maxd = max(maxd, up);
		}
		if (y < matrix[0].size() - 1 && matrix[x][y] > matrix[x][y + 1]) {
			int left = dfs_dp(matrix, dp, x, y + 1);
			maxd = max(maxd, left);
		}
		if (x < matrix.size() - 1 && matrix[x][y] > matrix[x + 1][y]) {
			int down = dfs_dp(matrix, dp, x + 1, y);
			maxd = max(maxd, down);
		}
		if (y > 0 && matrix[x][y] > matrix[x][y - 1]) {
			int right = dfs_dp(matrix, dp, x, y - 1);
			maxd = max(maxd, right);
		}
		dp[x][y] = maxd + 1;
		return dp[x][y];
	}
	int longestIncreasingPath(vector<vector<int>>& matrix) {
		int res = 0;
		if (matrix.empty() || matrix[0].empty()) return res;
		vector<vector<int>> dp(matrix.size(), vector<int>(matrix[0].size(), 0));
		for (int x = 0; x < matrix.size(); ++x)
		{
			for (int y = 0; y < matrix[x].size(); ++y)
			{
#if 0
				vector<vector<int>> bt(matrix.size(), vector<int>(matrix[0].size(), 0));
				int curres = 1;
				dfs_bt(matrix, bt, curres, res, x, y);
#endif
				res = max(dfs_dp(matrix, dp, x, y), res);
			}
		}
		return res;
	}
};

int main()
{
	Solution s;
	{
		vector<vector<int>> matrix{ { 9, 9, 4 }, { 6, 6, 8 }, { 2, 1, 1 } };
		cout << s.longestIncreasingPath(matrix) << endl;
	}

	std::cin.get();
	return 0;
}




