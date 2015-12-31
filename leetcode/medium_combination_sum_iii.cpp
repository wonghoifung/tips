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
	void combinationSum3Helper(vector<vector<int>>& res, int startidx, vector<int>& candidates, vector<int>& current, int currentsum, int k, int n) {
		if (currentsum > n) return;
		if (currentsum == n && current.size() == k)
		{
			res.push_back(current);
			return;
		}
		for (int i = startidx; i < candidates.size(); ++i) {
			if (candidates[i] + currentsum > n || current.size() == k) break;

			current.push_back(candidates[i]);
			combinationSum3Helper(res, i + 1, candidates, current, currentsum + candidates[i], k, n);
			current.pop_back();
		}
	}
	vector<vector<int>> combinationSum3(int k, int n) {
		vector<vector<int>> res;
		vector<int> current;
		vector<int> candidates{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		combinationSum3Helper(res, 0, candidates, current, 0, k, n);
		return res;
	}
};

int main()
{
	Solution solu;
	{
		vector<vector<int>> res = solu.combinationSum3(3, 9);
		for (const auto& v : res)
		{
			copy(v.begin(), v.end(), ostream_iterator<int>(cout, " ")); cout << endl;
		}
		cout << endl;
	}

	std::cin.get();
	return 0;
}




