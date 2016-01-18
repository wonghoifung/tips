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
	int jump(vector<int>& nums) {
		int step = 0;
		for (int l = 0, r = 0; r < nums.size() - 1; ++step)
		{
			int rNew = 0;
			for (int i = l; i <= r; i++)
			{
				rNew = max(rNew, i + nums[i]);
			}
			l = r + 1;
			r = rNew;
		}
		return step;
	}
};

int main()
{
	Solution s;
	{
		vector<int> nums{ 2, 3, 1, 1, 4 };
		cout << s.jump(nums) << endl;
	}

	std::cin.get();
	return 0;
}




