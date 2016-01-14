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
	vector<int> twoSum(vector<int>& nums, int target) {
		unordered_map<int, int> m;
		vector<int> res;
		for (int i = 0; i < nums.size(); ++i)
		{
			int idx = m[target - nums[i]];
			if (idx != 0)
			{
				res.push_back(idx);
				res.push_back(i + 1);
				return res;
			}
			m[nums[i]] = i + 1;
		}
		return res;
	}
	vector<int> twoSumSlow(vector<int>& nums, int target) {
		vector<int> res;
		for (int i = 0; i < nums.size(); ++i)
		{
			int nextTarget = target - nums[i];
			for (int j = i + 1; j < nums.size(); ++j)
			{
				if (nums[j] == nextTarget)
				{
					res.push_back(i + 1);
					res.push_back(j + 1);
					return res;
				}
			}
		}
		return res;
	}
};

int main()
{
	Solution s;
	{
		vector<int> nums{ 3, 2, 4 };
		vector<int> res = s.twoSum(nums, 6);
		copy(res.begin(), res.end(), ostream_iterator<int>(cout, " ")); cout << endl;
	}

	std::cin.get();
	return 0;
}




