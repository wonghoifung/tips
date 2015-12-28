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
	void permuteUniqueHelper(vector<int>& nums, vector<int>& current, vector<vector<int>>& res)
	{
		if (nums.size() == 0)
		{
			res.push_back(current);
			return;
		}

		unordered_map<int, int> mp;
		for (int i = 0; i < nums.size(); i++)
		{
			if (!mp[nums[i]])
			{
				current.push_back(nums[i]);
				vector<int> left;
				left = nums;
				left.erase(left.begin() + i);
				permuteUniqueHelper(left, current, res);
				current.pop_back();
				mp[nums[i]] = 1;
			}
		}
	}

	vector<vector<int>> permuteUnique(vector<int>& nums) {
		vector<vector<int>> res;
		vector<int> current;
		permuteUniqueHelper(nums, current, res);
		return res;
	}

	vector<vector<int>> permuteUniqueSTL(vector<int>& nums) {
		vector<vector<int>> res;
		sort(nums.begin(), nums.end());
		res.push_back(nums);
		while (next_permutation(nums.begin(),nums.end()))
		{
			res.push_back(nums);
		}
		return res;
	}
};

int main()
{
	Solution solu;
	{
		vector<int> nums{ 1, 1, 0, 0, 1, -1, -1, 1 };
		vector<vector<int>> res = solu.permuteUnique(nums);
		for (const auto& v : res)
		{
			copy(v.begin(), v.end(), ostream_iterator<int>(cout, " ")); cout << endl;
		}
	}

	std::cin.get();
	return 0;
}

