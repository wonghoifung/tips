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
		bool isValid(vector<int>& level, int idx) {
			return idx >= 0 && idx < level.size();
		}
		int value(vector<int>& level, int idx) {
			if (idx < 0 || idx >= level.size()) return 0;
			return level[idx];
		}
		int minimumTotal(vector<vector<int>>& triangle) {
			vector<vector<int>> dp(triangle);
			if (dp.size() == 1) return dp[0][0];
			int res = INT_MAX;
			for (int i = 1; i < dp.size(); ++i)
			{
				for (int j = 0; j < dp[i].size(); ++j)
				{
					auto& lastlevel = dp[i - 1];
					int cur = dp[i][j]; bool isChanged = false;
					if (isValid(lastlevel, j)) {
						isChanged = true;
						dp[i][j] = cur + value(lastlevel, j);
					}
					if (isValid(lastlevel, j - 1)) {
						if (isChanged) {
							dp[i][j] = min(dp[i][j], cur + value(lastlevel, j - 1));
						}
						else {
							isChanged = true;
							dp[i][j] = cur + value(lastlevel, j - 1);
						}
					}
					/*if (isValid(lastlevel, j + 1)) {
					  if (isChanged) {
					  dp[i][j] = min(dp[i][j], cur + value(lastlevel, j + 1));
					  }
					  else {
					  isChanged = true;
					  dp[i][j] = cur + value(lastlevel, j + 1);
					  }
					  }*/
					if (i == dp.size() - 1) res = min(res, dp[i][j]);
				}
			}
			return res;
		}
};

int main()
{
	Solution solu;
	{
		vector<vector<int>> triangle{
			{ 2 }, 
				{ 3, 4 },
				{ 6, 5, 7 },
				{ 4, 1, 8, 3 }
		};
		cout << solu.minimumTotal(triangle) << endl;
	}
	{
		vector<vector<int>> triangle{
			{ -1 },
				{ 2, 3 },
				{ 1, -1, -3 }
		};
		cout << solu.minimumTotal(triangle) << endl;
	}
	{
		vector<vector<int>> triangle{
			{ -10 }
		};
		cout << solu.minimumTotal(triangle) << endl;
	}
	{
		vector<vector<int>> triangle{
			{ -1 },
				{ 3, 2 },
				{ -3, 1, -1 }
		};
		cout << solu.minimumTotal(triangle) << endl;
	}
	std::cin.get();
	return 0;
}




