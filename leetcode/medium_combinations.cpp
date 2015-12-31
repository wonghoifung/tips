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

class mytimer
{
private:
	time_t start_;
public:
	mytimer()
	{
		start_ = time(nullptr);
	}
	~mytimer()
	{
		time_t end = time(nullptr);
		cout << end - start_ << "s" << endl;
	}
};

class Solution {
public:
	void combineHelper(vector<vector<int>>& res, vector<int>& current, int start, int n, int k) {
		if (k == 0)
		{
			res.push_back(current);
			return;
		}
		for (int i = start; i <= n; ++i)
		{
			current.push_back(i);
			combineHelper(res, current, i + 1, n, k - 1);
			current.pop_back();
		}
	}
	vector<vector<int>> combine(int n, int k) {
		vector<vector<int>> res;
		vector<int> current;
		combineHelper(res, current, 1, n, k);
		return res;
	}
};

int main()
{
	Solution solu;
	{
		vector<vector<int>> res = solu.combine(4, 2);
		for (const auto& v : res)
		{
			copy(v.begin(), v.end(), ostream_iterator<int>(cout, " ")); cout << endl;
		}
	}

	std::cin.get();
	return 0;
}




