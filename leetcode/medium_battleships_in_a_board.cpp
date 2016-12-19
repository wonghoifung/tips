#include <vector>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <sstream>
#include <algorithm>
#include <queue>
#include <set>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

class Solution {
public:
#if 0
	int countBattleships(vector<vector<char>>& board) {
		if (board.empty() || board[0].empty()) return 0;
		static const vector<pair<int, int>> dirs = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };
		int count = 0;
		int rowcnt = board.size();
		int colcnt = board[0].size();
		vector<vector<bool>> visited(board.size(), vector<bool>(board[0].size(), false));
		for (int x = 0; x < rowcnt; ++x) {
			for (int y = 0; y < colcnt; ++y) {
				if (visited[x][y]) continue;
				if (board[x][y] == '.') {
					visited[x][y] = true;
					continue;
				}
				count += 1;
				queue<pair<int, int>> q;
				q.push({ x, y });
				while (!q.empty()) {
					auto point = q.front(); q.pop();
					visited[point.first][point.second] = true;
					for (auto& d : dirs) {
						int xx = point.first + d.first;
						int yy = point.second + d.second;
						if (xx >= 0 && xx < rowcnt && yy >= 0 && yy < colcnt && !visited[xx][yy]) {
							if (board[xx][yy] == 'X') {
								q.push({ xx, yy });
							}
							else {
								visited[xx][yy] = true;
							}
						}
					}
				}
			}
		}

		return count;
	}
#endif
	int countBattleships(vector<vector<char>>& board) {
		int m = board.size();
		if (m == 0) return 0;
		int n = board[0].size();
		if (n == 0) return 0;
		int count = 0;
		for (int i = 0; i < m; ++i) {
			for (int j = 0; j < n; ++j) {
				if (board[i][j] == '.') continue;
				if (i > 0 && board[i - 1][j] == 'X') continue;
				if (j > 0 && board[i][j - 1] == 'X') continue;
				count += 1;
			}
		}
		return count;
	}
};

int main() {
	{

	}

	cin.get();
	return EXIT_SUCCESS;
}

