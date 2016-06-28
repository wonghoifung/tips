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
	bool isPerfectSquare(int num) { // newton method
		long r = num;
		while (r * r > num) {
			r = (r + num / r) / 2;
		}
		return r * r == num;
	}
};

int main()
{
	cout << boolalpha << Solution().isPerfectSquare(16) << endl;
	cout << boolalpha << Solution().isPerfectSquare(14) << endl;

	std::cin.get();
	return 0;
}




