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
	bool exist(vector<vector<char>>& board, string word) {
		if (board.empty() || board[0].empty() || word.empty()) return false;
		int rowCnt = board.size();
		int columnCnt = board[0].size();
		char first = word.at(0);
		for (size_t i = 0; i < rowCnt; ++i)
		{
			for (size_t j = 0; j < columnCnt; ++j)
			{
				if (board[i][j] == first)
				{
					if (dfs(board, i, j, word, 0))
					{
						return true;
					}
				}
			}
		}
		return false;
	}
	bool dfs(vector<vector<char>>& board, int i, int j, string& word, int idx) {
		if (word.size() == idx) return true;
		if (i < 0 || j < 0 || i >= board.size() || j >= board[0].size()) return false;
		if (board[i][j] == word.at(idx))
		{
			char tmp = board[i][j];
			board[i][j] = '.';

			if (dfs(board, i - 1, j, word, idx + 1)) return true;
			if (dfs(board, i + 1, j, word, idx + 1)) return true;
			if (dfs(board, i, j - 1, word, idx + 1)) return true;
			if (dfs(board, i, j + 1, word, idx + 1)) return true;

			board[i][j] = tmp;
		}
		return false;
	}

	enum class direction { up, right, down, left, none };
	enum { row, column };
	using position = tuple < int, int > ;
	vector<position> path;
	int rowCnt;
	int columnCnt;
	bool isInPath(position& pos) {
		for (const auto& p : path) {
			if (pos == p) return true;
		}
		return false;
	}
	const bool canGoUp(position& pos) const { return get<row>(pos) > 0; }
	const bool canGoRight(position& pos) const { return get<column>(pos) < columnCnt - 1; }
	const bool canGoDown(position& pos) const { return get<row>(pos) < rowCnt - 1; }
	const bool canGoLeft(position& pos) const { return get<column>(pos) > 0; }
	int i = 0; int j = 0; bool start = true;
	bool nextStartPos(vector<vector<char>>& board, char c, position& pos) {
		if (start) { 
			start = false; 
		}
		else { 
			j += 1; 
			if (j == columnCnt) {
				i += 1;
				j = 0;
			}
		}

		for (; i < rowCnt; ++i)
		{
			for (; j < columnCnt; ++j)
			{
				if (board[i][j] == c)
				{
					pos = position{ i, j };
					return true;
				}
			}
			j = 0;
		}
		return false;
	}
	bool existSlow(vector<vector<char>>& board, string word) {
		if (board.empty() || board[0].empty() || word.empty()) return false;
		rowCnt = board.size();
		columnCnt = board[0].size();
		i = 0; j = 0; start = true;
		position p;
		while (nextStartPos(board, word.at(0), p))
		{
			path.clear(); 
			path.push_back(p); 
			if (existHelper(board, word, 1, direction::none)) return true;
		}
		return false;
	}
	bool existHelper(vector<vector<char>>& board, string word, int idx, direction exceptDir) {
		if (word.size() == idx) return true;
		
		if (exceptDir != direction::up && canGoUp(path.back()) && board[get<row>(path.back()) - 1][get<column>(path.back())] == word.at(idx)) {
			position up{ get<row>(path.back()) - 1, get<column>(path.back()) };
			if (!isInPath(up))
			{
				path.push_back(up);
				if (existHelper(board, word, idx + 1, direction::down)) return true;
				path.pop_back();
			}
		}

		if (exceptDir != direction::right && canGoRight(path.back()) && board[get<row>(path.back())][get<column>(path.back()) + 1] == word.at(idx)) {
			position right{ get<row>(path.back()), get<column>(path.back()) + 1 };
			if (!isInPath(right))
			{
				path.push_back(right);
				if (existHelper(board, word, idx + 1, direction::left)) return true;
				path.pop_back(); 
			}
		}

		if (exceptDir != direction::down && canGoDown(path.back()) && board[get<row>(path.back()) + 1][get<column>(path.back())] == word.at(idx)) {
			position down{ get<row>(path.back()) + 1, get<column>(path.back()) };
			if (!isInPath(down))
			{
				path.push_back(down); 
				if (existHelper(board, word, idx + 1, direction::up)) return true;
				path.pop_back(); 
			}
		}

		if (exceptDir != direction::left && canGoLeft(path.back()) && board[get<row>(path.back())][get<column>(path.back()) - 1] == word.at(idx)) {
			position left{ get<row>(path.back()), get<column>(path.back()) - 1 };
			if (!isInPath(left))
			{
				path.push_back(left); 
				if (existHelper(board, word, idx + 1, direction::right)) return true;
				path.pop_back();
			}
		}

		return false;
	}
};

vector<char> fromString(const char* str) {
	vector<char> res;
	int len = strlen(str);
	for (size_t i = 0; i < len; ++i)
	{
		res.push_back(str[i]);
	}
	return res;
}

int main()
{
	Solution s;
	{
		vector<vector<char>> board{ fromString("ABCE"), fromString("SFCS"), fromString("ADEE") };
		string word = "ABCCED";
		cout << boolalpha << s.exist(board, word) << endl;
	}

	{
		vector<vector<char>> board{ fromString("aa") };
		string word = "aaa";
		cout << boolalpha << s.exist(board, word) << endl;
	}

	{
		vector<vector<char>> board{ fromString("ab"), fromString("cd") };
		string word = "cdba";
		cout << boolalpha << s.exist(board, word) << endl;
	}

	{
		vector<vector<char>> board{ fromString("a") };
		string word = "ab";
		cout << boolalpha << s.exist(board, word) << endl;
	}

	{
		vector<vector<char>> board{ fromString("a") };
		string word = "a";
		cout << boolalpha << s.exist(board, word) << endl;
	}

	std::cin.get();
	return 0;
}

