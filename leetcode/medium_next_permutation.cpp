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
	void nextPermutation(vector<int>& nums) {
		int r = nums.size() - 1;
		for (int i = nums.size() - 1; i > 0; --i)
		{
			if (nums[i] > nums[i - 1])
			{
				while (nums[r] <= nums[i - 1]) r -= 1;
				swap(nums[r], nums[i - 1]);
				sort(nums.begin() + i, nums.end());
				return;
			}
		}
		sort(nums.begin(), nums.end());
	}
};

int main()
{
	Solution s;
	{
		vector<int> nums{ 1, 3, 2 };
		s.nextPermutation(nums);
		copy(nums.begin(), nums.end(), ostream_iterator<int>(cout, " ")); cout << endl;
	}
	{
		vector<int> nums{ 1, 2, 3 };
		s.nextPermutation(nums);
		copy(nums.begin(), nums.end(), ostream_iterator<int>(cout, " ")); cout << endl;
	}
	{
		vector<int> nums{ 3, 2, 1 };
		s.nextPermutation(nums);
		copy(nums.begin(), nums.end(), ostream_iterator<int>(cout, " ")); cout << endl;
	}
	{
		vector<int> nums{ 1, 1, 5 };
		s.nextPermutation(nums);
		copy(nums.begin(), nums.end(), ostream_iterator<int>(cout, " ")); cout << endl;
	}

	std::cin.get();
	return 0;
}




