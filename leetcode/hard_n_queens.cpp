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

class mytimer
{
private:
	time_t start_;
public:
	mytimer()
	{
		start_ = time(nullptr);
	}
	~mytimer()
	{
		time_t end = time(nullptr);
		cout << end - start_ << "s" << endl;
	}
};

class Solution {
public:
	int N;
	vector<bool> rowflag;
	vector<bool> colflag;
	void initBoard(int n, vector<string>& board) {
		N = n;
		rowflag.resize(n, false);
		colflag.resize(n, false);
		board.resize(n, string(n, '.'));
	}
	bool isValid(int row, int col, vector<string>& board) {
		if (rowflag[row]) return false;
		if (colflag[col]) return false;
		for (int i = 0; i < N; ++i)
		{
			/* if (board[row][i] == 'Q' || board[i][col] == 'Q') { return false; } */
			if (col - i >= 0)
			{
				if (row - i >= 0 && board[row - i][col - i] == 'Q') { return false; }
				if (row + i < N && board[row + i][col - i] == 'Q') { return false; }
			}
			if (col + i < N)
			{
				if (row - i >= 0 && board[row - i][col + i] == 'Q') { return false; }
				if (row + i < N && board[row + i][col + i] == 'Q') { return false; }
			}
		}
		return true;
	}
	void solveNQueensHelper(int rownum, vector<string>& board, vector<vector<string>>& res) {
		if (rownum == N)
		{
			res.push_back(board);
			return;
		}
		for (int i = 0; i < N; ++i)
		{
			if (isValid(rownum, i, board))
			{
				board[rownum][i] = 'Q'; rowflag[rownum] = true; colflag[i] = true;
				solveNQueensHelper(rownum + 1, board, res);
				board[rownum][i] = '.'; rowflag[rownum] = false; colflag[i] = false;
			}
		}
	}
	vector<vector<string>> solveNQueens(int n) {
		vector<vector<string>> res;
		vector<string> board;
		initBoard(n, board);
		solveNQueensHelper(0, board, res);
		return res;
	}
};

int main()
{
	Solution solu;
	{
		vector<vector<string>> res = solu.solveNQueens(4);
		for (const auto& v : res)
		{
			copy(v.begin(), v.end(), ostream_iterator<string>(cout, "\n")); cout << endl;
		}
	}

	std::cin.get();
	return 0;
}

