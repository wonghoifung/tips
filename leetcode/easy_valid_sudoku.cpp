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
#include <sstream>
#include <algorithm>
#include <numeric>
#include <math.h>
#include <assert.h>
#include <time.h>

using namespace std;

class mytimer
{
private:
	time_t start_;
public:
	mytimer()
	{
		start_ = time(NULL);
	}
	~mytimer()
	{
		time_t end = time(NULL);
		cout << end - start_ << "s" << endl;
	}
};

class Solution {
public:
	struct context
	{
		unordered_map<int, unordered_set<char>> row_nums;
		unordered_map<int, unordered_set<char>> col_nums;
		unordered_map<int, unordered_set<char>> cell_nums;
	};
	int cellIndex(int x, int y)
	{
		return y / 3 + x / 3 * 3;
	}
	bool canSet(int x, int y, char c, context& ctx)
	{
		if (ctx.row_nums[x].find(c) != ctx.row_nums[x].end())
		{
			return false;
		}
		if (ctx.col_nums[y].find(c) != ctx.col_nums[y].end())
		{
			return false;
		}
		int cellidx = cellIndex(x, y);
		if (ctx.cell_nums[cellidx].find(c) != ctx.cell_nums[cellidx].end())
		{
			return false;
		}
		return true;
	}
	void setPos(int x, int y, char c, context& ctx)
	{
		ctx.row_nums[x].insert(c);
		ctx.col_nums[y].insert(c);
		ctx.cell_nums[cellIndex(x, y)].insert(c);
	}
	bool isValidSudoku(vector<vector<char>>& board) {
		char buf[9][9] = { 0 };
		context ctx;
		for (size_t i = 0; i < board.size(); ++i)
		{
			for (size_t j = 0; j < board[i].size(); ++j)
			{
				char c = board[i][j];
				if (c == '.')
				{
					continue;
				}
				if (canSet(i, j, c, ctx))
				{
					buf[i][j] = c;
					setPos(i, j, c, ctx);
				}
				else
				{
					return false;
				}
			}
		}
		return true;
	}
};

void print_board(vector<vector<char>>& board)
{
	for (size_t i = 0; i < board.size(); ++i)
	{
		for (size_t j = 0; j < board[i].size(); ++j)
		{
			cout << board[i][j] << " ";
		}
		cout << endl;
	}
}

void init_board(string (&rows)[9], vector<vector<char>>& board)
{
	for (size_t i = 0; i < 9; i++)
	{
		vector<char> r;
		for (size_t j = 0; j < 9; j++)
		{
			r.push_back(rows[i].at(j));
		}
		board.push_back(r);
	}
}

int main()
{
	Solution s;

	{
		mytimer t;
		string rows[] = { "..9748...", "7........", ".2.1.9...", "..7...24.", ".64.1.59.", ".98...3..", "...8.3.2.", "........6", "...2759.." };
		vector<vector<char>> board;
		init_board(rows, board);
		print_board(board);
		cout << "-----------------" << endl;
		cout << boolalpha << s.isValidSudoku(board) << endl;
	}

	std::cin.get();

	return 0;
}

