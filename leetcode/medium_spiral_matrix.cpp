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
	vector<int> spiralOrder(vector<vector<int>>& matrix) {
		vector<int> res;

		if (matrix.empty()) return res;

		int rowstart = 0;
		int rowend = matrix.size() - 1;
		int colstart = 0;
		int colend = matrix[0].size() - 1;
		
		vector<vector<bool>> flag(matrix.size(), vector<bool>(matrix[0].size(), false));
		while (true)
		{
			if (flag[rowstart][colstart]) break;
			for (int i = colstart; i <= colend; ++i)
			{
				res.push_back(matrix[rowstart][i]);
				flag[rowstart][i] = true;
			}
			rowstart += 1;
			if (rowstart > rowend) break;

			if (flag[rowstart][colend]) break;
			for (int i = rowstart; i <= rowend; ++i)
			{
				res.push_back(matrix[i][colend]);
				flag[i][colend] = true;
			}
			colend -= 1;
			if (colend < colstart) break;

			if (flag[rowend][colend]) break;
			for (int i = colend; i >= colstart; --i)
			{
				res.push_back(matrix[rowend][i]);
				flag[rowend][i] = true;
			}
			rowend -= 1;
			if (rowend < rowstart) break;

			if (flag[rowend][colstart]) break;
			for (int i = rowend; i >= rowstart; --i)
			{
				res.push_back(matrix[i][colstart]);
				flag[i][colstart] = true;
			}
			colstart += 1;
			if (colstart > colend) break;
		}
		
		return res;
	}
};

int main()
{
	Solution s;
	{
		vector<vector<int>> matrix {
			{ 1, 2, 3 },
			{ 4, 5, 6 },
			{ 7, 8, 9 }
		};
		vector<int> res = s.spiralOrder(matrix);
		copy(res.begin(), res.end(), ostream_iterator<int>(cout, " ")); cout << endl;
	}
	{
		vector<vector<int>> matrix{ { 1 } };
		vector<int> res = s.spiralOrder(matrix);
		copy(res.begin(), res.end(), ostream_iterator<int>(cout, " ")); cout << endl;
	}

	std::cin.get();
	return 0;
}




