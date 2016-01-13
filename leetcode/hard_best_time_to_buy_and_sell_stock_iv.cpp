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
	int maxProfit(int k, vector<int>& prices) {
		int n = prices.size();
		if (n <= 1) return 0;
		if (k >= n / 2)
		{
			int maxP = 0;
			for (int i = 1; i < n; ++i)
			{
				if (prices[i] > prices[i - 1])
					maxP += prices[i] - prices[i - 1];
			}
			return maxP;
		}
		vector<int> hold(k + 1, INT_MIN);
		vector<int> rele(k + 1, 0);
		int cur;
		for (int i = 0; i < n; ++i)
		{
			cur = prices[i];
			for (int j = k; j > 0; --j)
			{
				rele[j] = max(rele[j], hold[j] + cur);
				hold[j] = max(hold[j], rele[j - 1] - cur);
			}
		}
		return rele[k];
	}
};

int main()
{
	Solution s;
	{
		int k = 6;
		vector<int> prices{ 1, 3, 5 };
		cout << s.maxProfit(k, prices) << endl;
	}
	{
		int k = 2;
		vector<int> prices{ 6, 1, 3, 2, 4, 7 };
		cout << s.maxProfit(k, prices) << endl;
	}

	std::cin.get();
	return 0;
}




