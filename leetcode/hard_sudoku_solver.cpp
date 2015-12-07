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

class Solution
{
public:
	struct pos
	{
		int x;
		int y;
	};
	struct postry
	{
		int x;
		int y;
		char c;
	};
	struct context
	{
		unordered_map<int, unordered_set<char>> row_nums;
		unordered_map<int, unordered_set<char>> col_nums;
		unordered_map<int, unordered_set<char>> cell_nums;
		list<pos> pending_pos;
		stack<postry> trying_pos;
		vector<vector<char>>* pboard;
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
		(*ctx.pboard)[x][y] = c;
		ctx.row_nums[x].insert(c);
		ctx.col_nums[y].insert(c);
		ctx.cell_nums[cellIndex(x, y)].insert(c);
		ctx.trying_pos.push({ x, y, c });
	}
	void clrPos(context& ctx)
	{
		postry pt = ctx.trying_pos.top();
		int x = pt.x;
		int y = pt.y;
		char c = pt.c;
		ctx.trying_pos.pop();
		(*ctx.pboard)[x][y] = '.';
		ctx.row_nums[x].erase(c);
		ctx.col_nums[y].erase(c);
		ctx.cell_nums[cellIndex(x, y)].erase(c);
	}
	void init_context(vector<vector<char>>& board, context& ctx)
	{
		ctx.pboard = &board;
		for (size_t i = 0; i < board.size(); ++i)
		{
			for (size_t j = 0; j < board[i].size(); ++j)
			{
				char c = board[i][j];
				if (c != '.')
				{
					ctx.row_nums[i].insert(c);
					ctx.col_nums[j].insert(c);
					ctx.cell_nums[cellIndex(i, j)].insert(c);
				}
				else
				{
					ctx.pending_pos.push_back({ i, j });
				}
			}
		}
	}
	void solveSudoku(vector<vector<char>>& board)
	{
		context ctx;
		init_context(board, ctx);
		setPendingPos(ctx);
	}
	bool doNextTry(pos& p, context& ctx, char start)
	{
		for (char c = start; c <= '9'; ++c)
		{
			if (canSet(p.x, p.y, c, ctx))
			{
				setPos(p.x, p.y, c, ctx);
				return true;
			}
		}
		return false;
	}
	bool backtrack(context& ctx)
	{
		if (ctx.trying_pos.empty())
		{
			return false;
		}
		postry pt = ctx.trying_pos.top();
		clrPos(ctx);
		pos lastp = { pt.x, pt.y };
		ctx.pending_pos.push_front(lastp);
		bool ret = doNextTry(lastp, ctx, pt.c + 1);
		while (!ret)
		{
			if (!backtrack(ctx))
			{
				return false;
			}
			ret = doNextTry(lastp, ctx, '1');
		}
		ctx.pending_pos.pop_front();
		return true;
	}
	bool setPendingPos(context& ctx)
	{
		if (ctx.pending_pos.empty())
		{
			return true;
		}
		while (ctx.pending_pos.size())
		{
			pos p = ctx.pending_pos.front();
			while (!doNextTry(p, ctx, '1'))
			{
				if (!backtrack(ctx))
				{
					return false;
				}
			}
			ctx.pending_pos.pop_front();
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
		s.solveSudoku(board);
		print_board(board);
	}

	std::cin.get();

	return 0;
}

