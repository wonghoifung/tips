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
	int removeDuplicates(vector<int>& nums) {
		if (nums.empty()) return 0;
		int left = 0;
		int next = 1;
		while (next < nums.size())
		{
			if (nums[next] == nums[left]) next += 1;
			else swap(nums[next++], nums[++left]);
		}
		return left + 1;
	}
};

int main()
{
	Solution s;
	{
		vector<int> nums{ 1, 1, 2 };
		cout << s.removeDuplicates(nums) << endl;
	}

	std::cin.get();
	return 0;
}




