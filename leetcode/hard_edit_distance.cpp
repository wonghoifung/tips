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
	int minDistance(string word1, string word2) {
		int m = word1.size();
		int n = word2.size();
		vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

		for (int i = 0; i <= m; ++i) dp[i][0] = i;
		for (int i = 0; i <= n; ++i) dp[0][i] = i;
		for (int r = 1; r <= m; ++r)
		{
			for (int c = 1; c <= n; ++c)
			{
				if (word1[r - 1] == word2[c - 1])
				{
					dp[r][c] = dp[r - 1][c - 1];
				}
				else
				{
					dp[r][c] = 1 + min(min(dp[r - 1][c], dp[r][c - 1]), dp[r - 1][c - 1]);
				}
			}
		}
		return dp[m][n];
	}
};

int main()
{
	Solution s;
	{
		cout << s.minDistance("a", "a") << endl;
		cout << s.minDistance("a", "b") << endl;
		cout << s.minDistance("dinitrophenylhydrazine", "acetylphenylhydrazine") << endl;
	}

	std::cin.get();
	return 0;
}




