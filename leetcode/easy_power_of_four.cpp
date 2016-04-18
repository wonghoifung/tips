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
	bool isPowerOfFour(int num) {
		if (num < 0) return false;
		bool get1 = false;
		int cnt0 = 0;
		for (size_t i = 0; i < 32; i++)
		{
			if (num & 1) {
				if (get1) return false;
				get1 = true;
			}
			else {
				if (!get1) cnt0 += 1;
			}
			num >>= 1;
		}
		return get1 && cnt0 % 2 == 0;
	}
};

int main()
{
	Solution s;

	std::cin.get();
	return 0;
}




