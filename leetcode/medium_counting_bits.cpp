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
	vector<int> countBits(int num) {
		vector<int> res(num + 1, 0);
		for (int i = 1; i <= num; ++i) {
			/* res.push_back(__builtin_popcount(i)); */
			res[i] = res[i & (i - 1)] + 1;
		}
		return res;
	}
};

int main()
{
	Solution s;

	std::cin.get();
	return 0;
}




