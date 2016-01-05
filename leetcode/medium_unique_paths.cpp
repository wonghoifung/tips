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
	int calc(vector<vector<int>>& board, int r, int c) {
		if (r == 0 && c == 0) return 1;
		int up = 0;
		int left = 0;
		if (r > 0) up = board[r - 1][c];
		if (c > 0) left = board[r][c - 1];
		return up + left;
	}
	int uniquePaths(int m, int n) {
		vector<vector<int>> board(m, vector<int>(n, 0));
		for (int r = 0; r < m; ++r)
		{
			for (int c = 0; c < n; ++c)
			{
				board[r][c] = calc(board, r, c);
			}
		}
		return board[m - 1][n - 1];
	}
};

int main()
{
	Solution solu;
	{
		cout << solu.uniquePaths(3, 7) << endl; 
	}
	std::cin.get();
	return 0;
}




