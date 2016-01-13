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
	int coinChangeHelper(vector<int>& coins, int amount, vector<int>& count) {
		if (amount < 0) return -1;
		if (amount == 0) return 0;
		if (count[amount - 1] != 0) return count[amount - 1];
		int mincnt = INT_MAX;
		for (auto coin : coins)
		{
			int res = coinChangeHelper(coins, amount - coin, count);
			if (res >= 0 && res < mincnt) mincnt = 1 + res;
		}
		count[amount - 1] = mincnt == INT_MAX ? -1 : mincnt;
		return count[amount - 1];
	}
	int coinChange(vector<int>& coins, int amount) {
		if (amount < 1) return 0;
		vector<int> count(amount, 0);
		return coinChangeHelper(coins, amount, count);
	}
};

int main()
{
	Solution s;
	{
		vector<int> coins{ 1, 2, 5 };
		cout << s.coinChange(coins, 100) << endl;
	}

	std::cin.get();
	return 0;
}




