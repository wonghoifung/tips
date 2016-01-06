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

class NumMatrix {
public:
	vector<vector<int>> mat;

	int up(int r, int c) {
		if (r == 0) return 0;
		return mat[r - 1][c];
	}

	int left(int r, int c) {
		if (c == 0) return 0;
		return mat[r][c - 1];
	}

	int upLeft(int r, int c) {
		if (r == 0 || c == 0) return 0;
		return mat[r - 1][c - 1];
	}

	NumMatrix(vector<vector<int>>& matrix) :mat(matrix) {
		for (int r = 0; r < matrix.size(); ++r)
		{
			for (int c = 0; c < matrix[r].size(); ++c)
			{
				mat[r][c] += up(r, c) + left(r, c) - upLeft(r, c);
			}
		}
	}

	int sumRegion(int row1, int col1, int row2, int col2) {
		int res = mat[row2][col2];
		res -= up(row1, col2);
		res -= left(row2, col1);
		res += upLeft(row1, col1);
		return res;
	}
};

int main()
{
	vector<vector<int>> matrix{
		{ 3, 0, 1, 4, 2 },
		{ 5, 6, 3, 2, 1 },
		{ 1, 2, 0, 1, 5 },
		{ 4, 1, 0, 1, 7 },
		{ 1, 0, 3, 0, 5 }
	};

	NumMatrix numMatrix(matrix);

	cout << numMatrix.sumRegion(2, 1, 4, 3) << endl; // 8
	cout << numMatrix.sumRegion(1, 1, 2, 2) << endl; // 11
	cout << numMatrix.sumRegion(1, 2, 2, 4) << endl; // 12

	std::cin.get();
	return 0;
}




