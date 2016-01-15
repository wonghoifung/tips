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
	void moveZeroes(vector<int>& nums) {
		int zidx = 0;
		for (int i = 0; i < nums.size(); ++i)
		{
			if (nums[i] != 0) swap(nums[zidx++], nums[i]);
		}
	}
};

int main()
{
	Solution s;
	{
		vector<int> nums{ 0, 1, 0, 3, 12 };
		s.moveZeroes(nums);
		copy(nums.begin(), nums.end(), ostream_iterator<int>(cout, " ")); cout << endl;
	}

	std::cin.get();
	return 0;
}




