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
	int firstMissingPositive(vector<int>& nums) {
		if (nums.empty()) return 1;
		for (int i = 0; i < nums.size(); ++i)
		{
			if (i + 1 == nums[i]) continue;
			if (nums[i] - 1 >= nums.size()) continue;
			if (nums[nums[i] - 1] == nums[i]) continue;
			swap(nums[nums[i] - 1], nums[i]);
			i -= 1;
		}
		for (int i = 0; i < nums.size(); ++i)
		{
			if (nums[i] != i + 1) return i + 1;
		}
		return nums.back() + 1;
	}
};

int main()
{
	Solution s;
	{
		vector<int> nums{ 1 };
		cout << s.firstMissingPositive(nums) << endl;
	}

	std::cin.get();
	return 0;
}




