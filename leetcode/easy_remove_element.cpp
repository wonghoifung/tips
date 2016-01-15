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
	int removeElement(vector<int>& nums, int val) {
		if (nums.empty()) return 0;
		int idx = nums.size() - 1;
		int right = nums.size() - 1;
		while (idx >= 0 && right >= 0)
		{
			if (nums[idx] == val) { swap(nums[idx], nums[right]); right -= 1; }
			idx -= 1;
		}
		return right + 1;
	}
};

int main()
{
	Solution s;
	{
		vector<int> nums{ 4, 5 };
		cout << s.removeElement(nums, 5) << endl;
	}

	std::cin.get();
	return 0;
}




