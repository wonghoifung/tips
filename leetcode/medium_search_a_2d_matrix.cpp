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

class Solution {
public:
	bool searchMatrix(vector<vector<int>>& matrix, int target) {
		if (matrix.empty() || matrix[0].empty()) return false;
		int r = 0;
		int c = matrix[0].size() - 1;
		while (r < matrix.size() && c >= 0)
		{
			if (matrix[r][c] > target) c -= 1;
			else if (matrix[r][c] < target) r += 1;
			else return true;
		}
		return false;
	}
};

int main()
{
	Solution s;
	{
		vector<vector<int>> matrix{
			{ 1, 3, 5, 7 },
			{ 10, 11, 16, 20 },
			{23, 30, 34, 50 }
		};
		cout << boolalpha << s.searchMatrix(matrix, 3) << endl;
	}

	std::cin.get();
	return 0;
}




