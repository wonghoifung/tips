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
	int divide(int dividend, int divisor) {
		if (dividend == INT_MIN && divisor == -1) return INT_MAX;
		long divd = abs((long)dividend);
		long divs = abs((long)divisor);
		int res = 0;
		while (divd >= divs)
		{
			int cnt = 0;
			while (divd >= (divs << cnt))
			{
				cnt += 1;
			}
			cnt -= 1;
			res += 1 << cnt;
			divd -= divs << cnt;
		}
		if ((dividend > 0 && divisor > 0) || (dividend < 0 && divisor < 0)) return res;
		return -res;
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




