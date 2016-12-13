#include <vector>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <sstream>
#include <algorithm>
#include <queue>
using namespace std;

class Solution {
	class Cell {
	public:
		int row;
		int col;
		int height;
		Cell(int r, int c, int h) :row(r), col(c), height(h) {}
	};
	struct CellLess {
		bool operator()(const Cell& a, const Cell& b) {
			return a.height > b.height; // the smaller height gain more priority
		}
	};
public:
	int trapRainWater(vector<vector<int>>& heightMap) {
		if (heightMap.empty() || heightMap[0].empty()) return 0;
		priority_queue<Cell, vector<Cell>, CellLess> q;
		int m = heightMap.size();
		int n = heightMap[0].size();
		vector<vector<bool>> visited(m, vector<bool>(n, false));
		/* visit border cells */
		for (int i = 0; i < m; ++i) {
			visited[i][0] = true;
			visited[i][n - 1] = true;
			q.push(Cell(i, 0, heightMap[i][0]));
			q.push(Cell(i, n - 1, heightMap[i][n - 1]));
		}
		for (int i = 0; i < n; ++i) {
			visited[0][i] = true;
			visited[m - 1][i] = true;
			q.push(Cell(0, i, heightMap[0][i]));
			q.push(Cell(m - 1, i, heightMap[m - 1][i]));
		}
		vector<vector<int>> dirs = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } }; // top bottom left right
		int res = 0;
		while (!q.empty()) {
			Cell cell = q.top(); q.pop(); // pick the lowest cell visited and remove it
			for (auto& dir : dirs) { // check all its neighbors
				int row = cell.row + dir[0];
				int col = cell.col + dir[1];
				if (row >= 0 && row < m && col >= 0 && col < n && !visited[row][col]) {
					visited[row][col] = true;
					res += max(0, cell.height - heightMap[row][col]); // trap water when meet a lower neighbor
					q.push(Cell(row, col, max(heightMap[row][col], cell.height))); // push the new visited cell with height updated
				}
			}
		}
		return res;
	}
};

int main() {
	{
		vector<vector<int>> heightMap = { 
			{ 1, 4, 3, 1, 3, 2 },
			{ 3, 2, 1, 3, 2, 4 },
			{ 2, 3, 3, 2, 3, 1 } };
		cout << Solution().trapRainWater(heightMap) << endl; // 4
	}

	cin.get();
	return EXIT_SUCCESS;
}

