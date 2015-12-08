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
#include <sstream>
#include <algorithm>
#include <numeric>
#include <math.h>
#include <assert.h>
#include <time.h>

using namespace std;

class mytimer
{
private:
	time_t start_;
public:
	mytimer()
	{
		start_ = time(NULL);
	}
	~mytimer()
	{
		time_t end = time(NULL);
		cout << end - start_ << "s" << endl;
	}
};

class Solution {
public:
	void rotate(vector<vector<int>>& matrix) {
		if (matrix.empty())
		{
			return;
		}
		if (matrix[0].empty())
		{
			return;
		}
		vector<vector<int>> tmp = matrix;
		matrix.clear();
		for (size_t i = 0; i < tmp[0].size(); ++i)
		{
			matrix.push_back(vector<int>());
			for (size_t j = 0; j < tmp.size(); ++j)
			{
				matrix.back().push_back(tmp[j][i]);
			}
			reverse(matrix.back().begin(), matrix.back().end());
		}
	}
};

int main()
{
	Solution s;
	{
		vector<vector<int>> v = { { 1, 2 }, { 3, 4 } };
		s.rotate(v);
		for (auto& r : v)
		{
			for (auto& field : r)
			{
				cout << field << " ";
			}
			cout << endl;
		}
	}
	std::cin.get();

	return 0;
}

