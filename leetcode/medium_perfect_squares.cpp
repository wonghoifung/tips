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
	int numSquares(int n) {
		vector<int> dp(n + 1, INT_MAX);
		dp[0] = 0;
		for (int i = 1; i <= n; ++i)
		{
			int sqr = sqrt(i);
			if (sqr * sqr == i) 
			{
				dp[i] = 1;
				continue;
			}
			for (int j = 1; j <= sqr; ++j)
			{
				dp[i] = min(dp[i], (dp[i - j * j] + 1));
			}
		}
		return dp[n];
	}
};

int main()
{
	Solution s;
	{
		cout << s.numSquares(13) << endl; // 2 4+9
		cout << s.numSquares(12) << endl; // 3 4+4+4
	}

	std::cin.get();
	return 0;
}




