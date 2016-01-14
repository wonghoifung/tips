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
	void setZeroes(vector<vector<int>>& matrix) {
		unordered_set<int> rows;
		unordered_set<int> cols;
		for (int r = 0; r < matrix.size(); ++r)
		{
			for (int c = 0; c < matrix[r].size(); ++c)
			{
				if (matrix[r][c] == 0)
				{
					rows.insert(r);
					cols.insert(c);
				}
			}
		}
		for (auto r : rows)
		{
			for (int c = 0; c < matrix[r].size(); ++c)
			{
				matrix[r][c] = 0;
			}
		}
		for (auto c : cols)
		{
			for (int r = 0; r < matrix.size(); ++r)
			{
				matrix[r][c] = 0;
			}
		}
	}
};

int main()
{
	Solution s;
	{

	}

	std::cin.get();
	return 0;
}




