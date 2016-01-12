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
	int numDistinct(string s, string t) {
		vector<int> dp(t.size() + 1, 0);
		dp[0] = 1;
		for (auto c : s) {
			for (int i = t.size() - 1; i >= 0; --i)
			{
				if (c == t.at(i)) dp[i + 1] += dp[i];
			}
		}
		return dp.back();
	}
};

int main()
{
	Solution s;
	{
		cout << s.numDistinct("rabbbit", "rabbit") << endl; // 3
	}

	std::cin.get();
	return 0;
}




