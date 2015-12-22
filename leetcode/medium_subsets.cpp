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
#include <memory>
#include <sstream>
#include <algorithm>
#include <bitset>
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
	void subsetsHelper(vector<vector<int>>& res, vector<int>& list, vector<int>& nums, int idx) {
		if (idx == nums.size())
		{
			res.push_back(list);
			return;
		}

		subsetsHelper(res, list, nums, idx + 1);

		list.push_back(nums[idx]);
		subsetsHelper(res, list, nums, idx + 1);
		list.pop_back();
	}
	vector<vector<int>> subsets(vector<int>& nums) {
		vector<vector<int>> res;
		if (nums.empty()) return res;
		sort(nums.begin(), nums.end());
		vector<int> list;
		subsetsHelper(res, list, nums, 0);
		return res;
	}
	vector<vector<int>> subsets2(vector<int>& nums) {
		vector<vector<int>> res;
		if (nums.empty()) return res;
		sort(nums.begin(), nums.end());
		int maxnum = pow(2, nums.size());
		for (size_t i = 0; i < maxnum; ++i)
		{
			vector<int> v;
			for (size_t j = 0; j < nums.size(); ++j)
			{
				if ((1 << j) & i)
				{
					v.push_back(nums[j]);
				}
			}
			res.push_back(v);
		}
		return res;
	}
	vector<vector<int>> subsets1(vector<int>& nums) {
		vector<vector<int>> res;
		if (nums.empty()) return res;
		sort(nums.begin(), nums.end());
		res.push_back({});
		for (size_t idx = 0; idx < nums.size(); ++idx)
		{
			int newnum = nums[idx];
			int currentlen = res.size();
			for (size_t i = 0; i < currentlen; i++)
			{
				vector<int> v = res[i];
				v.push_back(newnum);
				res.push_back(v);
			}
		}
		return res;
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

