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
	vector<vector<int>> dir;
public:
	Solution() :dir({ { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } }) {
		/* dir: down up right left */
	}
	vector<pair<int, int>> pacificAtlantic(vector<vector<int>>& matrix) {
		vector<pair<int, int>> res;
		if (matrix.empty() || matrix[0].empty()) return res;
		int n = matrix.size();
		int m = matrix[0].size();
		vector<vector<bool>> pacific(n, vector<bool>(m, false));
		vector<vector<bool>> atlantic(n, vector<bool>(m, false));
		queue<pair<int, int>> pQ;
		queue<pair<int, int>> aQ;
		for (int i = 0; i < n; ++i) {
			pQ.push({ i, 0 });
			aQ.push({ i, m - 1 });
			pacific[i][0] = true;
			atlantic[i][m - 1] = true;
		}
		for (int i = 0; i < m; ++i) {
			pQ.push({ 0, i });
			aQ.push({ n - 1, i });
			pacific[0][i] = true;
			atlantic[n - 1][i] = true;
		}
		bfs(matrix, pQ, pacific);
		bfs(matrix, aQ, atlantic);
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				if (pacific[i][j] && atlantic[i][j]) res.push_back({ i, j });
			}
		}
		return res;
	}
	void bfs(vector<vector<int>>& matrix, queue<pair<int, int>>& q, vector<vector<bool>>& visited) {
		int n = matrix.size();
		int m = matrix[0].size();
		while (!q.empty()) {
			auto cur = q.front(); q.pop();
			for (auto& d : dir) {
				int x = cur.first + d[0];
				int y = cur.second + d[1];
				if (x < 0 || x >= n || y < 0 || y >= m || visited[x][y] ||
					matrix[x][y] < matrix[cur.first][cur.second]) {
					continue;
				}
				visited[x][y] = true;
				q.push({ x, y });
			}
		}
	}
};

int main() {
	{
		vector<vector<int>> matrix = {
			{ 1, 2, 2, 3, 5 },
			{ 3, 2, 3, 4, 4 },
			{ 2, 4, 5, 3, 1 },
			{ 6, 7, 1, 4, 5 },
			{ 5, 1, 1, 2, 4 }
		};
		auto res = Solution().pacificAtlantic(matrix);
		for (auto& cord : res) {
			cout << "[" << cord.first << ", " << cord.second << "]" << " ";
		}
		cout << endl;
	}

	cin.get();
	return EXIT_SUCCESS;
}

