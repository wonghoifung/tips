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
	int lengthOfLIS(vector<int>& nums) {
		if (nums.empty()) return 0;
		int maxlen = 1;
		vector<int> lesscnt(nums.size(), 1);
		for (int i = 1; i < nums.size(); ++i)
		{
			for (int j = i - 1; j >= 0; --j)
			{
				if (nums[i] > nums[j])
				{
					lesscnt[i] = max(lesscnt[i], lesscnt[j] + 1);
					maxlen = max(maxlen, lesscnt[i]);
				}
			}
		}
		return maxlen;
	}
};

int main()
{
	Solution s;
	{
		vector<int> nums{ 1, 3, 6, 7, 9, 4, 10, 5, 6 };
		cout << s.lengthOfLIS(nums) << endl;
	}

	std::cin.get();
	return 0;
}




