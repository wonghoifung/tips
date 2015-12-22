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
	vector<int> singleNumber(vector<int>& nums) {
		int xorVal = nums[0];
		for (size_t i = 1; i < nums.size(); ++i)
		{
			xorVal ^= nums[i];
		}

		int diffbit = xorVal;
		for (int i = 0; i < 32; ++i)
		{
			if (diffbit & (1 << i))
			{
				diffbit = i;
				break;
			}
		}

		int n1 = 0;
		for (size_t idx = 0; idx < nums.size(); ++idx)
		{
			if (nums[idx] & (1 << diffbit))
			{
				n1 ^= nums[idx];
			}
		}
		int n2 = n1 ^ xorVal;
		return { n1, n2 };
	}
};

int main()
{
	Solution s;
	{
		vector<int> nums{ 0, 0, 1, 2 };
		vector<int> res = s.singleNumber(nums);
		copy(res.begin(), res.end(), ostream_iterator<int>(cout, " ")); cout << endl;
	}

	std::cin.get();
	return 0;
}

