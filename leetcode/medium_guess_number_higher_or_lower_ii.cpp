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
#include <stdarg.h>
#include <stdio.h>

using namespace std;

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))
class Solution {
public:
	int getMoneyAmount(int n) {
		if (n == 1) return 0;
		vector<vector<int>> dp(n + 1, vector<int>(n + 1, 0));
		for (int i = 1; i < n; ++i) {
			for (int beg = 0; beg + i <= n; ++beg) {
				int end = beg + i;
				dp[beg][end] = INT_MAX;
				for (int k = beg; k <= end; ++k) { // choose k and k is not the picked one, pay price k
					dp[beg][end] = MIN(dp[beg][end],
							   k + MAX(k - 1 >= beg ? dp[beg][k - 1] : 0, 
								   end >= k + 1 ? dp[k + 1][end] : 0));
				}
			}
		}
		return dp[1][n];
	}
};

int main()
{
	cout << Solution().getMoneyAmount(10) << endl;

	std::cin.get();
	return 0;
}




