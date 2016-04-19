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

using namespace std;

class Solution {
public:
	int integerBreak(int n) { // https://leetcode.com/discuss/98173/o-log-n-time-solution-with-explanation
		if (n == 2) return 1;
		if (n == 3) return 2;
		vector<int> prod(n + 1, 0);
		prod[2] = 2;
		prod[3] = 3;
		for (int i = 4; i <= n; ++i) {
			prod[i] = max(prod[i - 2] * prod[2], prod[i - 3] * prod[3]);
		}
		return prod[n];
	}
};

int main()
{
	Solution s;
	for (int i = 2; i <= 10; i++)
	{
		cout << s.integerBreak(i) << endl;
	}

	std::cin.get();
	return 0;
}




