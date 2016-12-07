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
#include <stdio.h>

using namespace std;

class Solution {
public:
	int findNthDigit(int n) {
		long base = 9, digits = 1;
		while (n - base * digits > 0) {
			n -= base * digits;
			base *= 10;
			digits += 1;
		}

		int index = n % digits;
		if (index == 0) index = digits;
		long num = 1;
		for (int i = 1; i < digits; ++i) num *= 10;
		num += (index == digits) ? n / digits - 1 : n / digits;

		for (int i = index; i < digits; ++i)
			num /= 10;
		return num % 10;
	}
};

int main()
{
	cout << Solution().findNthDigit(3) << endl;
	cout << Solution().findNthDigit(11) << endl;
	cout << Solution().findNthDigit(100000000) << endl;

	std::cin.get();
	return 0;
}



