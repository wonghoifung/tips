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
	int countNumbersWithUniqueDigits(int n) {
		int num = 0;
		for (auto i = n; i >= 1; --i) {
			int part = 9;
			for (auto j = 0; j < i - 1; ++j) {
				part *= (9 - j);
			}
			num += part;
		}
		return num + 1;
	}
};

int main()
{
	cout << Solution().countNumbersWithUniqueDigits(0) << endl;
	cout << Solution().countNumbersWithUniqueDigits(1) << endl;
	cout << Solution().countNumbersWithUniqueDigits(2) << endl;
	cout << Solution().countNumbersWithUniqueDigits(3) << endl;
	cout << Solution().countNumbersWithUniqueDigits(4) << endl;

	std::cin.get();
	return 0;
}




