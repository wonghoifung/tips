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
#include <stdarg.h>
#include <stdio.h>

using namespace std;

class Solution {
public:
	int wiggleMaxLength(vector<int>& nums) {
		if (nums.empty()) return 0;
		vector<int> up(nums.size(), 0);
		vector<int> down(nums.size(), 0);
		up[0] = 1;
		down[0] = 1;
		for (int i = 1; i < nums.size(); ++i) {
			if (nums[i] > nums[i - 1]) {
				up[i] = down[i - 1] + 1;
				down[i] = down[i - 1];
			}
			else if (nums[i] < nums[i - 1]) {
				down[i] = up[i - 1] + 1;
				up[i] = up[i - 1];
			}
			else {
				down[i] = down[i - 1];
				up[i] = up[i - 1];
			}
		}
		return down.back() > up.back() ? down.back() : up.back();
	}
};

int main()
{
	{
		vector<int> nums{ 1, 7, 4, 9, 2, 5 };
		cout << Solution().wiggleMaxLength(nums) << endl; // 6
	}
	{
		vector<int> nums{ 1, 17, 5, 10, 13, 15, 10, 5, 16, 8 };
		cout << Solution().wiggleMaxLength(nums) << endl; // 7
	}
	{
		vector<int> nums{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		cout << Solution().wiggleMaxLength(nums) << endl; // 2
	}
	std::cin.get();
	return 0;
}




