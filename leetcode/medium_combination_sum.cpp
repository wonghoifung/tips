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
	void combinationSumHelper(vector<vector<int>>& res, int startidx, vector<int>& candidates, vector<int>& current, int currentsum, int target) {
		if (currentsum == target)
		{
			res.push_back(current);
			return;
		}
		for (int i = startidx; i < candidates.size(); ++i) {
			if (candidates[i] + currentsum > target) break;	
			current.push_back(candidates[i]);
			combinationSumHelper(res, i, candidates, current, currentsum + candidates[i], target);
			current.pop_back();
		}
	}
	vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
		vector<vector<int>> res;
		vector<int> current;
		sort(candidates.begin(), candidates.end());
		combinationSumHelper(res, 0, candidates, current, 0, target);
		return res;
	}
};

int main()
{
	Solution solu;
	{
		vector<int> candidates{ 2, 3, 6, 7 };
		int target = 7;
		vector<vector<int>> res = solu.combinationSum(candidates, target);
		for (const auto& v : res)
		{
			copy(v.begin(), v.end(), ostream_iterator<int>(cout, " ")); cout << endl;
		}
	}

	std::cin.get();
	return 0;
}




