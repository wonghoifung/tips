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
	int maxSubArray(vector<int>& nums) {
		int maxs = INT_MIN;
		int sum = 0;
		for (size_t i = 0; i < nums.size(); ++i)
		{
			if (sum < 0) sum = 0;
			sum += nums[i];
			maxs = max(sum, maxs);
		}
		return maxs;
	}
};

int main()
{
	Solution solu;
	{
		vector<int> nums{ -1 };
		cout << solu.maxSubArray(nums) << endl;
	}

	std::cin.get();
	return 0;
}




