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
	vector<vector<int>> generate(int numRows) {
		vector<vector<int>> res;
		if (numRows == 0) return res;
		if (numRows > 0) res.push_back({ 1 });
		if (numRows > 1) res.push_back({ 1, 1 });
		if (numRows > 2) {
			int r = 3;
			while (r <= numRows)
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
		}
		return res;
	}
};

int main()
{
	Solution s;
	{
		auto res = s.generate(5);
		for (const auto& v : res){
			copy(v.begin(), v.end(), ostream_iterator<int>(cout, " ")); 
			cout << endl;
		}
	}

	std::cin.get();
	return 0;
}




