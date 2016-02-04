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

void print_board(vector<vector<char>>& board) {
	for (const auto& v : board) {
		copy(v.begin(), v.end(), ostream_iterator<char>(cout, " "));
		cout << endl;
	}
}

class Solution {
public:
	void solve(vector<vector<char>>& board) {
		if (board.size() < 2 || board[0].size() < 2) return;
		int rowcnt = board.size();
		int colcnt = board[0].size();

		queue<pair<int,int>> q;
		for (int c = 0; c < colcnt; ++c)
		{
			if (board[0][c] == 'O') q.push({ 0, c });
			if (board[rowcnt - 1][c] == 'O') q.push({ rowcnt - 1, c });
		}
		for (int r = 0; r < rowcnt; ++r)
		{
			if (board[r][0] == 'O') q.push({ r, 0 });
			if (board[r][colcnt - 1] == 'O') q.push({ r, colcnt - 1 });
		}

		while (!q.empty())
		{
			auto p = q.front();
			q.pop();
			if (board[p.first][p.second] == 'O') {
				board[p.first][p.second] = 'B';
				if (p.first > 0) q.push({ p.first - 1, p.second });
				if (p.first < rowcnt - 1) q.push({ p.first + 1, p.second });
				if (p.second > 0) q.push({ p.first, p.second - 1 });
				if (p.second < colcnt - 1) q.push({ p.first, p.second + 1 });
			}
		}

		for (int r = 0; r < rowcnt; ++r)
		{
			for (int c = 0; c < colcnt; ++c)
			{
				if (board[r][c] == 'O') board[r][c] = 'X';
				else if (board[r][c] == 'B') board[r][c] = 'O';
			}
		}
	}
};

int main()
{
	Solution s;
	{
		vector<vector<char>> board{ { 'X', 'O', 'X' }, { 'X', 'O', 'X' }, { 'X', 'O', 'X' } };
		s.solve(board);
		print_board(board);
	}

	std::cin.get();
	return 0;
}




