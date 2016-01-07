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
	int maximalSquare(vector<vector<char>>& matrix) {
		if (matrix.empty()) return 0;
		int m = matrix.size();
		int n = matrix[0].size();
		int edge = 0;
		vector<vector<int>> board(m + 1, vector<int>(n + 1, 0));
		for (int  i = 1; i <= m; ++i)
		{
			for (int j = 1; j <= n; ++j)
			{
				if (matrix[i - 1][j - 1] == '1')
				{
					board[i][j] = min(min(board[i][j - 1], board[i - 1][j - 1]), board[i - 1][j]) + 1;
					edge = max(edge, board[i][j]);
				}
			}
		}
		return edge * edge;
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




