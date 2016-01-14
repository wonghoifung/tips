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
	vector<vector<int>> generateMatrix(int n) {
		vector<vector<int>> res(n, vector<int>(n, 0));
		if (n == 0) return res;

		int rowstart = 0;
		int rowend = n - 1;
		int colstart = 0;
		int colend = n - 1;

		int value = 1;
		while (true)
		{
			if (res[rowstart][colstart]) break;
			for (int i = colstart; i <= colend; ++i)
			{
				res[rowstart][i] = value++;
			}
			rowstart += 1;
			if (rowstart > rowend) break;

			if (res[rowstart][colend]) break;
			for (int i = rowstart; i <= rowend; ++i)
			{
				res[i][colend] = value++;
			}
			colend -= 1;
			if (colend < colstart) break;

			if (res[rowend][colend]) break;
			for (int i = colend; i >= colstart; --i)
			{
				res[rowend][i] = value++;
			}
			rowend -= 1;
			if (rowend < rowstart) break;

			if (res[rowend][colstart]) break;
			for (int i = rowend; i >= rowstart; --i)
			{
				res[i][colstart] = value++;
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
		vector<vector<int>> res = s.generateMatrix(3);
		for (const auto& v : res) {
			copy(v.begin(), v.end(), ostream_iterator<int>(cout, " ")); cout << endl;
		}
	}

	std::cin.get();
	return 0;
}




