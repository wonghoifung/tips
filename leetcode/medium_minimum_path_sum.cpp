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
	int calc(vector<vector<int>>& grid, int r, int c) {
		if (r == 0 && c == 0) return grid[r][c];
		int up = INT_MAX;
		int left = INT_MAX;
		if (r > 0) up = grid[r - 1][c];
		if (c > 0) left = grid[r][c - 1];
		return min(up, left) + grid[r][c];
	}
	int minPathSum(vector<vector<int>>& grid) {
		int m = grid.size();
		int n = grid[0].size();
		for (int r = 0; r < m; ++r)
		{
			for (int c = 0; c < n; ++c)
			{
				grid[r][c] = calc(grid, r, c);
			}
		}
		return grid[m - 1][n - 1];
	}
};

int main()
{
	Solution s;
	{
	}

	std::cin.get();
	return 0;
}




