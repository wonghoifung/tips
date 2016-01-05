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
	int calc(vector<vector<int>>& board, int r, int c, vector<vector<int>>& obstacleGrid) {
		if (obstacleGrid[r][c] == 1) return 0;
		if (r == 0 && c == 0) return 1;
		int up = 0;
		int left = 0;
		if (r > 0) up = board[r - 1][c];
		if (c > 0) left = board[r][c - 1];
		return up + left;
	}
	int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
		if (obstacleGrid.empty() || obstacleGrid[0].empty()) return 0;
		int m = obstacleGrid.size();
		int n = obstacleGrid[0].size();
		vector<vector<int>> board(m, vector<int>(n, 0));
		for (int r = 0; r < m; ++r)
		{
			for (int c = 0; c < n; ++c)
			{
				board[r][c] = calc(board, r, c, obstacleGrid);
			}
		}
		return board[m - 1][n - 1];
	}
};

int main()
{
	Solution solu;
	{
		vector<vector<int>> obstacleGrid{
			{ 0, 0, 0 },
			{ 0, 1, 0 },
			{ 0, 0, 0 }
		};
		cout << solu.uniquePathsWithObstacles(obstacleGrid) << endl;
	}
	std::cin.get();
	return 0;
}




