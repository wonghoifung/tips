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
	int maximalRectangle(vector<vector<char>>& matrix) {
		if (matrix.empty()) return 0;
		int n = matrix[0].size();
		vector<vector<int>> board(matrix.size(), vector<int>(n, 0));
		int res = 0;
		for (int r = 0; r < matrix.size(); r++)
		{
			vector<int> height(n, 0);
			for (int c = 0; c < matrix[r].size(); c++)
			{
				if (r == 0 && matrix[r][c] == '1') {
					board[r][c] = 1; 
					height[c] = 1;
				}
				else if (r != 0 && matrix[r][c] == '1') {
					board[r][c] = board[r - 1][c] + 1;
					height[c] = board[r][c];
				}
			}
			res = max(res, largestRectangleArea(height));
		}
		return res;
	}
	int largestRectangleArea(vector<int>& height) {
		int n = height.size();
		stack<int> s;
		int maxa = 0;
		int tp;
		int tpa;
		int i = 0;
		while (i < n)
		{
			if (s.empty() || height[s.top()] <= height[i]) s.push(i++);
			else {
				tp = s.top();
				s.pop();
				tpa = height[tp] * (s.empty() ? i : i - s.top() - 1);
				maxa = max(maxa, tpa);
			}
		}
		while (!s.empty())
		{
			tp = s.top();
			s.pop();
			tpa = height[tp] * (s.empty() ? i : i - s.top() - 1);
			maxa = max(maxa, tpa);
		}
		return maxa;
	}
};

int main()
{
	Solution s;
	{
		vector<vector<char>> matrix{ { '0', '1' }, { '1', '0' } };
		cout << s.maximalRectangle(matrix) << endl;
	}

	std::cin.get();
	return 0;
}




