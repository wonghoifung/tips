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
	vector<int> getRow(int rowIndex) {
		vector<vector<int>> res;
		if (rowIndex == 0) return { 1 };
		if (rowIndex == 1) return { 1, 1 };
		res.push_back({ 1 });
		res.push_back({ 1, 1 });
		int r = 3;
		while (r <= rowIndex + 1)
		{
			vector<int> level;
			for (int i = 0; i < r; ++i)
			{
				if (i == 0) { level.push_back(1); }
				else if (i == r - 1) { level.push_back(1); }
				else { level.push_back(res.back()[i - 1] + res.back()[i]); }
			}
			res.push_back(level);
			r += 1;
		}
		return res.back();
	}
};

int main()
{
	Solution s;
	{
		auto v = s.getRow(3);
		copy(v.begin(), v.end(), ostream_iterator<int>(cout, " "));
		cout << endl;
	}

	std::cin.get();
	return 0;
}




