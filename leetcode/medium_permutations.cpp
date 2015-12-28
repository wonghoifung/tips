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
	void permuteHelper(vector<int>& nums, vector<bool>& flags, vector<int>& current, vector<vector<int>>& res) {
		if (current.size() == nums.size())
		{
			res.push_back(current);
		}
		for (size_t i = 0; i < flags.size(); ++i)
		{
			if (flags[i] == false)
			{
				current.push_back(nums[i]);
				flags[i] = true;
				permuteHelper(nums, flags, current, res);
				current.pop_back();
				flags[i] = false;
			}
		}
	}
	vector<vector<int>> permute(vector<int>& nums) {
		vector<vector<int>> res;
		if (nums.empty()) return res;
		vector<int> current;
		vector<bool> flags(nums.size(), false);
		permuteHelper(nums, flags, current, res);
		return res;
	}

	vector<vector<int>> permuteSTL(vector<int>& nums) {
		vector<vector<int>> res;
		sort(nums.begin(), nums.end());
		res.push_back(nums);
		while (next_permutation(nums.begin(), nums.end()))
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
		vector<int> nums{ 0, -1, 1 };
		vector<vector<int>> res = solu.permute(nums);
		for (const auto& v : res)
		{
			copy(v.begin(), v.end(), ostream_iterator<int>(cout, " ")); cout << endl;
		}
	}

	std::cin.get();
	return 0;
}

