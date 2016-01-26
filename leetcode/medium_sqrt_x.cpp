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
	int mySqrt(int x) {
		if (x == 0) return 0;
		int start = 1;
		int end = x;
		while (start < end)
		{
			int mid = start + (end - start) / 2;
			if (mid <= x / mid && (mid + 1) > x / (mid + 1)) return mid;
			else if (mid > x / mid) end = mid;
			else start = mid + 1;
		}
		return start;
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




