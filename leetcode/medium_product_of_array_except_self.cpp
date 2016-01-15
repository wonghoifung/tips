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
	vector<int> productExceptSelf(vector<int>& nums) {
		int n = nums.size();
		vector<int> res(n, 1);
		for (int i = 1; i < n; ++i)
		{
			res[i] = res[i - 1] * nums[i - 1];
		}
		int tmp = 1;
		for (int i = n - 1; i >= 0; --i)
		{
			res[i] *= tmp;
			tmp *= nums[i];
		}
		return res;
	}
	vector<int> productExceptSelfDiv(vector<int>& nums) {
		int product = 1;
		int zerocnt = 0;
		for (auto i : nums)
		{
			if (i != 0) product *= i;
			else zerocnt += 1;
		}
		if (zerocnt > 1) return vector<int>(nums.size(), 0);
		vector<int> res;
		for (int i = 0; i < nums.size(); ++i)
		{
			if (zerocnt == 0)
			{
				res.push_back(product / nums[i]);
			}
			else
			{
				res.push_back(nums[i] != 0 ? 0 : product);
			}
		}
		return res;
	}
};

int main()
{
	Solution s;
	{
		vector<int> nums{ 1, 2, 3, 4 }; //[24,12,8,6]
		auto res = s.productExceptSelf(nums);
		copy(res.begin(), res.end(), ostream_iterator<int>(cout, " ")); cout << endl;
	}

	std::cin.get();
	return 0;
}




