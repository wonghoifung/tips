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
	void combinationSum2Helper(vector<vector<int>>& res, int startidx, vector<int>& candidates, vector<int>& current, int currentsum, int target) {
		if (currentsum > target) return;
		if (currentsum == target)
		{
			res.push_back(current);
			return;
		}
		for (int i = startidx; i < candidates.size(); ++i) {
			if (candidates[i] + currentsum > target) break;	

			int repeated = 0;
			while (i + repeated + 1 < candidates.size())
			{
				if (candidates[i] != candidates[i + repeated + 1]) break;
				repeated += 1;
			}
			
			int cnt = repeated + 1;
			for (int j = 1; j <= cnt; ++j)
			{
				for (int k = 0; k < j; ++k) current.push_back(candidates[i]);
				combinationSum2Helper(res, i + cnt, candidates, current, currentsum + candidates[i] * j, target);
				for (int k = 0; k < j; ++k) current.pop_back();
			}
			i += repeated;
		}
	}
	vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
		vector<vector<int>> res;
		vector<int> current;
		sort(candidates.begin(), candidates.end());
		combinationSum2Helper(res, 0, candidates, current, 0, target);
		return res;
	}
};

int main()
{
	Solution solu;
	{
		vector<int> candidates{ 10, 1, 2, 7, 6, 1, 5 };
		int target = 8;
		vector<vector<int>> res = solu.combinationSum2(candidates, target);
		for (const auto& v : res)
		{
			copy(v.begin(), v.end(), ostream_iterator<int>(cout, " ")); cout << endl;
		}
		cout << endl;
	}
	{
		vector<int> candidates{ 2, 2, 2 };
		int target = 4;
		vector<vector<int>> res = solu.combinationSum2(candidates, target);
		for (const auto& v : res)
		{
			copy(v.begin(), v.end(), ostream_iterator<int>(cout, " ")); cout << endl;
		}
		cout << endl;
	}

	std::cin.get();
	return 0;
}




