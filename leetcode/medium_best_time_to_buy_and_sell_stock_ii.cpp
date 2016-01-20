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
	int maxProfit(vector<int>& prices) {
		int p = 0;
		for (int i = 1; i < prices.size(); ++i)
		{
			if (prices[i] - prices[i - 1] > 0) {
				p += (prices[i] - prices[i - 1]);
			}
		}
		return p;
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




