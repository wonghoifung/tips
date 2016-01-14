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
	int trap(vector<int>& height) {
		int traps = 0;
		if (height.size() < 3) return traps;
		int left = 0;
		int cur = 0;
		for (int i = 1; i < height.size(); ++i)
		{
			if (height[i] < height[left])
			{
				cur += (height[left] - height[i]);
			}
			else if (height[i] >= height[left] && i - left > 1)
			{
				traps += cur;
				cur = 0;
				left = i;
			}
			else
			{
				cur = 0;
				left = i;
			}
		}
		if (cur > 0 && height.size() - 1 - left > 1)
		{
			int right = height.size() - 1;
			int rcur = 0;
			for (int i = height.size() - 2; i >= left; --i)
			{
				if (height[i] < height[right])
				{
					rcur += (height[right] - height[i]);
				}
				else if (height[i] >= height[right] && right - i > 1)
				{
					traps += rcur;
					rcur = 0;
					right = i;
				}
				else
				{
					rcur = 0;
					right = i;
				}
			}
		}
		return traps;
	}
};

int main()
{
	Solution s;
	{
		vector<int> height{ 2, 1, 0, 2 };
		cout << s.trap(height) << endl;
	}
	{
		vector<int> height{ 4, 2, 3 };
		cout << s.trap(height) << endl;
	}

	std::cin.get();
	return 0;
}




