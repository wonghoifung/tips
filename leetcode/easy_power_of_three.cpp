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
	bool isPowerOfThree(int n) {
		if (n == 1) return true;
		long p = 3;
		while (p < n)
		{
			p *= 3;
		}
		return p == n;
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




