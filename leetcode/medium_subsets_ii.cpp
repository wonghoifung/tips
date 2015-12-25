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
	vector<vector<int>> subsetsWithDup(vector<int>& nums) {
		vector<vector<int>> res;
		if (nums.empty()) return res;
		sort(nums.begin(), nums.end());
		res.push_back({});

		for (size_t idx = 0; idx < nums.size(); ++idx)
		{
			int newnum = nums[idx];
			
			int dupcnt = 0;
			for (size_t i = idx + 1; i < nums.size(); ++i)
			{
				if (nums[i] == newnum) dupcnt += 1;
				if (nums[i] != newnum) break;
			}
			idx += dupcnt;

			int currentlen = res.size();
			for (size_t i = 0; i < currentlen; i++)
			{
				vector<int> v = res[i];
				v.push_back(newnum);
				res.push_back(v);

				for (size_t j = 0; j < dupcnt; j++)
				{
					v.push_back(newnum);
					res.push_back(v);
				}
			}
		}
		return res;
	}
};

int main()
{
	Solution solu;

	std::cin.get();
	return 0;
}

