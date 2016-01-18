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

/*
 * Any live cell with fewer than two live neighbors dies, as if caused by under-population.
 * Any live cell with two or three live neighbors lives on to the next generation.
 * Any live cell with more than three live neighbors dies, as if by over-population..
 * Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
 * */
class Solution {
public:
	int rcnt;
	int ccnt;
	int liveCountAround(vector<vector<int>>& board, int r, int c) {
		int livecnt = 0;
		if (r > 0 && board[r - 1][c]) livecnt += 1;
		if (r > 0 && c > 0 && board[r - 1][c - 1]) livecnt += 1;
		if (c > 0 && board[r][c - 1]) livecnt += 1;
		if (r < rcnt - 1 && c > 0 && board[r + 1][c - 1]) livecnt += 1;
		if (r < rcnt - 1 && board[r + 1][c]) livecnt += 1;
		if (r < rcnt - 1 && c < ccnt - 1 && board[r + 1][c + 1]) livecnt += 1;
		if (c < ccnt - 1 && board[r][c + 1]) livecnt += 1;
		if (r > 0 && c < ccnt - 1 && board[r - 1][c + 1]) livecnt += 1;
		return livecnt;
	}
	int cellNext(vector<vector<int>>& board, int r, int c) {
		int livecnt = liveCountAround(board, r, c);
		if (livecnt < 2) return 0;
		else if (livecnt == 2) return board[r][c];
		else if (livecnt == 3) return 1;
		else return 0; 
	}
	void gameOfLife(vector<vector<int>>& board) {
		if (board.empty()) return;
		rcnt = board.size();
		ccnt = board[0].size();
		vector<pair<int, int>> changes;
		for (int r = 0; r < rcnt; ++r)
		{
			for (int c = 0; c < ccnt; ++c)
			{
				if (cellNext(board, r, c) != board[r][c])
				{
					changes.push_back({ r, c });
				}
			}
		}
		for (const auto& p : changes)
		{
			board[p.first][p.second] = board[p.first][p.second] == 1 ? 0 : 1;
		}
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




