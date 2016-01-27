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
	int trailingZeroes(int n) {
		int cnt = 0;
		for (long long i = 5; n / i; i *= 5)
			cnt += n / i;
		return cnt;
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




