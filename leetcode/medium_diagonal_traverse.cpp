#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <stdint.h>

using namespace std;

class Solution {
public:
	vector<int> findDiagonalOrder(vector<vector<int>>& matrix) {
		vector<int> res;
		if (matrix.empty()) return res;
		int m = matrix.size();
		int n = matrix[0].size();
		res = vector<int>(m * n, 0);
		int row = 0, col = 0, d = 0;
		int dirs[2][2] = {
			{ -1, 1 },
			{ 1, -1 }
		};
		for (int i = 0; i < m * n; ++i) {
			res[i] = matrix[row][col];
			row += dirs[d][0];
			col += dirs[d][1];
			if (row >= m) { row = m - 1; col += 2; d = 1 - d; }
			else if (col >= n) { col = n - 1; row += 2; d = 1 - d; }
			else if (row < 0) { row = 0; d = 1 - d; }
			else if (col < 0) { col = 0; d = 1 - d; }
		}
		return res;
	}
};

int main() {
	{
		vector<vector<int>> matrix{
			{ 1, 2, 3 },
			{ 4, 5, 6 },
			{ 7, 8, 9 }
		};
		auto res = Solution().findDiagonalOrder(matrix);
		for (int i : res) cout << i << " ";
		cout << endl;
	}

	cin.get();
}

