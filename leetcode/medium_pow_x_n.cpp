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
	double myPow(double x, int n) {
		double res = 1;
		if (n < 0) {
			n *= -1;
			x = 1 / x;
		}
		while (n > 0)
		{
			if (n & 1) res *= x;
			x = x * x;
			n >>= 1;
		}
		return res;
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




