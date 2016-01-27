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
	int countDigitOne(int n) {
		if (n <= 0) return 0;
		int q = n;
		int x = 1;
		int res = 0;
		do {
			int d = q % 10;
			q /= 10;
			res += q * x;
			if (d == 1) res += n % x + 1;
			if (d > 1) res += x;
			x *= 10;
		} while (q > 0);
		return res;
	}
};

int main()
{
	Solution s;
	{
		cout << s.countDigitOne(824883294) << endl;
	}

	std::cin.get();
	return 0;
}




