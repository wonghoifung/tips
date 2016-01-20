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
	int maxArea(vector<int>& height) {
		if (height.size() < 2) return 0;
		int i = 0;
		int j = height.size() - 1;
		int area = min(height[i], height[j]) * (j - i);
		while (i < j)
		{
			if (height[i] < height[j]) {
				int t = height[i];
				while (++i < height.size())
				{
					if (height[i] <= t) continue;
					if (i >= j) return area;
					area = max(area, min(height[i], height[j])*(j - i));
					break;
				}
			}
			else {
				int t = height[j];
				while (--j >= 0)
				{
					if (height[j] <= t) continue;
					if (i >= j) return area;
					area = max(area, min(height[i], height[j])*(j - i));
					break;
				}
			}

		}
		return area;
	}
};

int main()
{
	Solution s;
	{
	
	}

	std::cin.get();
	return 0;
}




