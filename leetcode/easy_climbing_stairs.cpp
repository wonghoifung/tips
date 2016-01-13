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
	int climbStairs(int n) {
		vector<int> cnts(n + 1, 0);
		cnts[1] = 1;
		cnts[2] = 2;
		for (int i = 3; i <= n; ++i)
		{
			cnts[i] = cnts[i - 1] + cnts[i - 2];
		}
		return cnts[n];
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




